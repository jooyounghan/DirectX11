#include "PickableCamera.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DirectXDevice.h"

#include <algorithm>

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
	), IRectangle(uiWidthIn, uiHeightIn)
{
	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice, uiWidthIn, uiHeightIn,
		1, 0, D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT, eRTVFormatIn, cpMSToSSTexture.GetAddressOf()
	);
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpMSToSSTexture.Get(), cpMStoSSSRV.GetAddressOf());
}

PickableCamera::~PickableCamera()
{
}

void PickableCamera::ClearRTV()
{
	DirectXDevice::pDeviceContext->ClearRenderTargetView(IRenderTarget::cpRTV.Get(), IRenderTarget::fClearColor);

	const float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	for (IFilter* filter : pFilters)
	{
		DirectXDevice::pDeviceContext->ClearUnorderedAccessViewFloat(filter->cpUAV.Get(), clearColor);
	}
	DirectXDevice::pDeviceContext->ClearUnorderedAccessViewFloat(cpFormatChangedUAV.Get(), clearColor);
}

void PickableCamera::ClearDSV()
{
	DirectXDevice::pDeviceContext->ClearDepthStencilView(cpDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.f, NULL);
}

void PickableCamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	if (IsSwapChainAccesssed)
	{
		uiWidth = uiWidthIn;
		uiHeight = uiHeightIn;

		sTexelSize.fTexelXSize = 1.f / (float)uiWidth;
		sTexelSize.fTexelYSize = 1.f / (float)uiHeight;

		// Depth Stencil 초기화
		cpDSV.Reset();
		IDepthStencil::cpTexture2D.Reset();

		// Render Target 초기화
		IRenderTarget::cpSRV.Reset();
		IRenderTarget::cpRTV.Reset();
		IRenderTarget::cpTexture2D.Reset();

		// 백버퍼 및 Swap Chain 사이즈 변경
		ASwapChainAccessable::cpTexture2D.Reset();
		DirectXDevice::pSwapChain->ResizeBuffers(0, uiWidth, uiHeight, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		SetAsSwapChainBackBuffer();


		// Render Target 재설정
		ID3D11Helper::CreateTexture2D(
			DirectXDevice::pDevice,
			uiWidth, uiHeight,
			IRenderTarget::uiArraySize, IRenderTarget::uiNumQualityLevels,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			NULL, NULL, D3D11_USAGE_DEFAULT, IRenderTarget::eFormat, IRenderTarget::cpTexture2D.GetAddressOf()
		);
		ID3D11Helper::CreateRenderTargetView(DirectXDevice::pDevice, IRenderTarget::cpTexture2D.Get(), IRenderTarget::cpRTV.GetAddressOf());
		ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, IRenderTarget::cpTexture2D.Get(), IRenderTarget::cpSRV.GetAddressOf());

		// Depth Stencil 재설정
		ID3D11Helper::CreateTexture2D(
			DirectXDevice::pDevice,
			uiWidth, uiHeight,
			IDepthStencil::uiArraySize, IDepthStencil::uiNumQualityLevels,
			D3D11_BIND_DEPTH_STENCIL,
			NULL, NULL, D3D11_USAGE_DEFAULT, IDepthStencil::eFormat, IDepthStencil::cpTexture2D.GetAddressOf()
		);
		ID3D11Helper::CreateDepthStencilView(DirectXDevice::pDevice, IDepthStencil::cpTexture2D.Get(), cpDSV.GetAddressOf());

		// MS To SS 초기화
		cpMStoSSSRV.Reset();
		cpMSToSSTexture.Reset();

		// MS TO SS 재설정
		ID3D11Helper::CreateTexture2D(
			DirectXDevice::pDevice, uiWidth, uiHeight,
			1, 0, D3D11_BIND_SHADER_RESOURCE,
			NULL, NULL, D3D11_USAGE_STAGING, IRenderTarget::eFormat, cpMSToSSTexture.GetAddressOf()
		);
		ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpMSToSSTexture.Get(), cpMStoSSSRV.GetAddressOf());

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
				NULL, D3D11_USAGE_DEFAULT, IRenderTarget::eFormat, filter->cpTexture2D.GetAddressOf()
			);
			ID3D11Helper::CreateUnorderedAccessView(
				DirectXDevice::pDevice, 
				filter->cpTexture2D.Get(),
				filter->cpUAV.GetAddressOf()
			);
		}

		ID3D11Helper::UpdateBuffer(DirectXDevice::pDeviceContext, sTexelSize, D3D11_MAP_WRITE_DISCARD, cpTexelSize.Get());
	}
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
		if (ASwapChainAccessable::uiNumQualityLevels == IRenderTarget::uiNumQualityLevels)
		{
			ppInputSRV = IRenderTarget::cpSRV.GetAddressOf();
			pOutputResource = IRenderTarget::cpTexture2D.Get();
		}
		else
		{
			DirectXDevice::pDeviceContext->ResolveSubresource(cpMSToSSTexture.Get(), 0, IRenderTarget::cpTexture2D.Get(), 0, IRenderTarget::eFormat);
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

void PickableCamera::SetMousePos(const int& iMouseX, const int& iMouseY)
{
	const float fWidth = (float)uiWidth - 1.f;
	const float fHeight = (float)uiHeight - 1.f;
	const float fMouseX = std::clamp((float)iMouseX, 0.f, fWidth);
	const float fMouseY = std::clamp((float)iMouseY, 0.f, fHeight);

	sMousePosNdc.fMouseXNdc = (fMouseX * 2.f / fWidth) - 1.f;
	sMousePosNdc.fMouseXNdc = (fMouseY * 2.f / fHeight) - 1.f;

	ID3D11Helper::UpdateBuffer(DirectXDevice::pDeviceContext, sMousePosNdc, D3D11_MAP_WRITE_DISCARD, cpMousePosNdc.Get());
}

DirectX::XMMATRIX PickableCamera::GetTranformMat()
{
	return MathematicalHelper::MakeAffineTransformation(
		1.f, 1.f, 1.f,
		sAngles.fPitch, sAngles.fYaw, sAngles.fRoll,
		xmvPosition.m128_f32[0], xmvPosition.m128_f32[1], xmvPosition.m128_f32[2]
	);
}