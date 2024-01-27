#include "FilteredCamera.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

FilteredCamera::FilteredCamera(
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
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		(float)uiWidthIn, (float)uiHeightIn,
		fFovRadIn, fNearZIn, fFarZIn
	),
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(uiWidthIn, uiHeightIn)
{
}

FilteredCamera::~FilteredCamera()
{
}

void FilteredCamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	ACamera::Resize(uiWidthIn, uiHeightIn);

	for (AFilter* filter : pFilters)
	{
		// 필터 초기화
		filter->cpUAV.ReleaseAndGetAddressOf();
		filter->cpSRV.ReleaseAndGetAddressOf();
		filter->cpTexture2D.ReleaseAndGetAddressOf();

		filter->cpUAV = nullptr;
		filter->cpSRV = nullptr;
		filter->cpTexture2D = nullptr;

		// 필터 재설정
		filter->uiWidth = uiWidthIn;
		filter->uiHeight = uiHeightIn;
		filter->uiArraySize = 1;
		filter->uiNumQualityLevels = 0;

		ID3D11Helper::CreateTexture2D(DirectXDevice::pDevice,
			filter->uiWidth, filter->uiHeight,
			filter->uiArraySize, 0,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, NULL,
			NULL, D3D11_USAGE_DEFAULT, RenderTarget::eFormat, filter->cpTexture2D.GetAddressOf()
		);
		ID3D11Helper::CreateUnorderedAccessView(
			DirectXDevice::pDevice,
			filter->cpTexture2D.Get(),
			filter->cpUAV.GetAddressOf()
		);
	}
}

void FilteredCamera::Resolve()
{
	ID3D11ShaderResourceView** ppInputSRV = nullptr;
	ID3D11Texture2D* pOutputResource = nullptr;

	if (p_back_buffer != nullptr)
	{
		D3D11_TEXTURE2D_DESC back_buffer_desc;
		p_back_buffer->GetDesc(&back_buffer_desc);

		if (back_buffer_desc.SampleDesc.Quality != RenderTarget::uiNumQualityLevels ||
			back_buffer_desc.Format != RenderTarget::eFormat
			)
		{
			Apply(RenderTarget::cpSRV.GetAddressOf());
			ppInputSRV = AFilter::cpSRV.GetAddressOf();
			pOutputResource = AFilter::cpTexture2D.Get();
		}
		else
		{
			ppInputSRV = RenderTarget::cpSRV.GetAddressOf();
			pOutputResource = RenderTarget::cpTexture2D.Get();

		}

		if (pFilters.size() > 0)
		{
			for (AFilter* pFilter : pFilters)
			{
				pFilter->Apply(ppInputSRV);
				ppInputSRV = pFilter->cpSRV.GetAddressOf();
			}

			pOutputResource = pFilters[pFilters.size() - 1]->cpTexture2D.Get();
		}

		DirectXDevice::pDeviceContext->ResolveSubresource(p_back_buffer, 0, pOutputResource, 0, back_buffer_desc.Format);
	}

}
