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
	if (uiNumQualityLevelsIn > 0)
	{
		ID3D11Helper::CreateTexture2D(
			DirectXDevice::pDevice, uiWidthIn, uiHeightIn,
			1, 0, D3D11_BIND_SHADER_RESOURCE,
			NULL, NULL, D3D11_USAGE_DEFAULT, eRTVFormatIn, cpMSToSSTexture.GetAddressOf()
		);
		ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpMSToSSTexture.Get(), cpMStoSSSRV.GetAddressOf());
	}
}

AFilteredCamera::~AFilteredCamera()
{
}

void AFilteredCamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	ACamera::Resize(uiWidthIn, uiHeightIn);

	if (IsSwapChainAccesssed)
	{
		// MSToSS 초기화 및 재설정
		if (ARenderTarget::uiNumQualityLevels > 0)
		{
			cpMStoSSSRV.Reset();
			cpMSToSSTexture.Reset();

			ID3D11Helper::CreateTexture2D(
				DirectXDevice::pDevice, uiWidth, uiHeight,
				1, 0, D3D11_BIND_SHADER_RESOURCE,
				NULL, NULL, D3D11_USAGE_STAGING, ARenderTarget::eFormat, cpMSToSSTexture.GetAddressOf()
			);
			ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpMSToSSTexture.Get(), cpMStoSSSRV.GetAddressOf());
		}

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
