#include "AFilteredCamera.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

AFilteredCamera::AFilteredCamera(
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
	: ACamera(
		fXPos, fYPos, fZPos,
		uiWidthIn, uiHeightIn,
		fFovRadIn, fNearZIn, fFarZIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn, eDSVFormatIn
	)
{
}

AFilteredCamera::~AFilteredCamera()
{
}

void AFilteredCamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	ACamera::Resize(uiWidthIn, uiHeightIn);

	if (IsSwapChainAccesssed)
	{
		for (IFilter* filter : pFilters)
		{
			// 필터 초기화
			filter->cpUAV.Reset();
			filter->cpSRV.Reset();
			filter->cpTexture2D.Reset();

			// 필터 재설정
			filter->uiWidth = uiWidthIn;
			filter->uiHeight = uiHeightIn;
			filter->uiArraySize = 1;
			filter->uiNumQualityLevels = 0;

			ID3D11Helper::CreateTexture2D(DirectXDevice::pDevice,
				filter->uiWidth, filter->uiHeight,
				filter->uiArraySize, 0,
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, NULL,
				NULL, D3D11_USAGE_DEFAULT, ARenderTarget::eFormat, filter->cpTexture2D.GetAddressOf()
			);
			ID3D11Helper::CreateUnorderedAccessView(
				DirectXDevice::pDevice,
				filter->cpTexture2D.Get(),
				filter->cpUAV.GetAddressOf()
			);
		}
	}
}

void AFilteredCamera::Resolve()
{
	if (IsSwapChainAccesssed)
	{
		ID3D11ShaderResourceView** ppInputSRV = nullptr;
		ID3D11Texture2D* pOutputResource = nullptr;

		if (IsDiffWithBackBuffer())
		{
			Apply(ARenderTarget::cpSRV.GetAddressOf());
			ppInputSRV = IFilter::cpSRV.GetAddressOf();
			pOutputResource = IFilter::cpTexture2D.Get();
		}
		else
		{
			ppInputSRV = ARenderTarget::cpSRV.GetAddressOf();
			pOutputResource = ARenderTarget::cpTexture2D.Get();

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

		DirectXDevice::pDeviceContext->ResolveSubresource(ASwapChainAccessable::cpTexture2D.Get(), 0, pOutputResource, 0, ASwapChainAccessable::eFormat);
	}
	else
	{

	}
}
