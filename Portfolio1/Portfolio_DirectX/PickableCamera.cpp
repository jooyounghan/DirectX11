#include "PickableCamera.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DirectXDevice.h"

using namespace std;

PickableCamera::PickableCamera(

	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovRadIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn,
	DXGI_FORMAT eDSVFormatIn
)
	: AFilteredCamera(
		fXPos, fYPos, fZPos,
		uiWidthIn, uiHeightIn,
		fFovRadIn, fNearZIn, fFarZIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn, eDSVFormatIn
	), IDPickableRenderTarget(uiWidthIn, uiHeightIn, uiNumQualityLevelsIn),
	ARectangle(uiWidthIn, uiHeightIn)
{

}

PickableCamera::~PickableCamera()
{
}

void PickableCamera::ClearRTV()
{
	DirectXDevice::pDeviceContext->ClearRenderTargetView(ARenderTarget::cpRTV.Get(), ARenderTarget::fClearColor);

	for (IFilter* filter : pFilters)
	{
		DirectXDevice::pDeviceContext->ClearUnorderedAccessViewFloat(filter->cpUAV.Get(), ARenderTarget::fClearColor);
	}
	DirectXDevice::pDeviceContext->ClearUnorderedAccessViewFloat(cpFormatChangedUAV.Get(), ARenderTarget::fClearColor);

	DirectXDevice::pDeviceContext->ClearRenderTargetView(IDPickableRenderTarget::cpRTV.Get(), ARenderTarget::fClearColor);
}

void PickableCamera::ClearDSV()
{
	DirectXDevice::pDeviceContext->ClearDepthStencilView(cpDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.f, NULL);
}

void PickableCamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	AFilteredCamera::Resize(uiWidthIn, uiHeightIn);
	IDPickableRenderTarget::Resize(uiWidthIn, uiHeightIn);
}

void PickableCamera::UpdateCamera(const float& fDelta)
{
	UpdateViewProj();
}

void PickableCamera::Resolve()
{
	if (IsSwapChainAccesssed)
	{
		ID3D11ShaderResourceView** ppInputSRV;
		ID3D11Texture2D* pOutputResource;
		if (ASwapChainAccessable::uiNumQualityLevels == ARenderTarget::uiNumQualityLevels)
		{
			ppInputSRV = ARenderTarget::cpSRV.GetAddressOf();
			pOutputResource = ARenderTarget::cpTexture2D.Get();
		}
		else
		{
			DirectXDevice::pDeviceContext->ResolveSubresource(cpMSToSSTexture.Get(), 0, ARenderTarget::cpTexture2D.Get(), 0, ARenderTarget::eFormat);
			ppInputSRV = cpMStoSSSRV.GetAddressOf();
			pOutputResource = cpMSToSSTexture.Get();
		}

		if (pFilters.size() > 0)
		{
			for (IFilter* pFilter : pFilters)
			{
				pFilter->Apply(ppInputSRV);
				ppInputSRV = pFilter->cpSRV.GetAddressOf();
			}

			pOutputResource = pFilters[pFilters.size() - 1]->cpTexture2D.Get();
		}

		D3D11_TEXTURE2D_DESC sOutputDesc;
		pOutputResource->GetDesc(&sOutputDesc);

		if (ASwapChainAccessable::eFormat != sOutputDesc.Format)
		{
			// cpFormatChangedUAV를 Output으로 포맷 변경



			pOutputResource = cpFormatChangedTexture.Get();
		}
		DirectXDevice::pDeviceContext->ResolveSubresource(ASwapChainAccessable::cpTexture2D.Get(), 0, pOutputResource, 0, ASwapChainAccessable::eFormat);
	}
	else
	{
		cpFormatChangedUAV.Reset();
		cpFormatChangedTexture.Reset();
	}
}

DirectX::XMMATRIX PickableCamera::GetTranformMat()
{
	return MathematicalHelper::MakeAffineTransformation(
		1.f, 1.f, 1.f,
		sAngles.fPitch, sAngles.fYaw, sAngles.fRoll,
		xmvPosition.m128_f32[0], xmvPosition.m128_f32[1], xmvPosition.m128_f32[2]
	);
}

uint32_t PickableCamera::GetPickedID()
{
	D3D11_BOX sBox;
	AutoZeroMemory(sBox);
	sBox.left = sMousePosNdc.uiMouseXNdc;
	sBox.right = sBox.left + 1;
	sBox.top = sMousePosNdc.uiMouseYNdc;
	sBox.bottom = sBox.top + 1;
	sBox.front = 0;
	sBox.back = 1;

	DirectXDevice::pDeviceContext->CopySubresourceRegion(
		cpPickedIDSelected.Get(), NULL, 0, 0, 0, 
		IDPickableRenderTarget::cpTexture2D.Get(), 0, &sBox
	);

	uint32_t uiResult = 0;
	ID3D11Texture2D* pReferenceIDTexture = cpPickedIDSelected.Get();

	if (ARenderTarget::uiNumQualityLevels > 0)
	{
		DirectXDevice::pDeviceContext->ResolveSubresource(cpPickedIDMStoSS.Get(), 0, cpPickedIDSelected.Get(), 0, IDPickableRenderTarget::eFormat);
		pReferenceIDTexture = cpPickedIDMStoSS.Get();
	}

	D3D11_MAPPED_SUBRESOURCE sMappedSubResource;
	AutoZeroMemory(sMappedSubResource);
	DirectXDevice::pDeviceContext->Map(pReferenceIDTexture, 0, D3D11_MAP_READ, NULL, &sMappedSubResource);
	memcpy(&uiResult, sMappedSubResource.pData, sizeof(uint32_t));
	DirectXDevice::pDeviceContext->Unmap(pReferenceIDTexture, 0);

	return uiResult;
}
