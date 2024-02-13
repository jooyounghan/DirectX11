#include "FilteredCamera.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "BlurFilter.h"

FilteredCamera::FilteredCamera(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn,
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
		fPitchDegIn, fYawDegIn, fRollDegIn,
		uiWidthIn, uiHeightIn,
		fFovRadIn, fNearZIn, fFarZIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn, eDSVFormatIn
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		fPitchDegIn, fYawDegIn, fRollDegIn,
		(float)uiWidthIn, (float)uiHeightIn,
		fFovRadIn, fNearZIn, fFarZIn
	),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
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

	for (std::unique_ptr<AFilter>& filter : upFilters)
	{
		// 필터 초기화
		filter->ResetFilter();
		filter->ResetSRV();
		filter->ResetTexture();

		// 필터 재설정
		filter->uiWidth = uiWidthIn;
		filter->uiHeight = uiHeightIn;

		ID3D11Helper::CreateTexture2D(DirectXDevice::pDevice,
			filter->uiWidth, filter->uiHeight,
			filter->GetArraySize(), filter->GetQualityLevels(),
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, NULL,
			NULL, D3D11_USAGE_DEFAULT, RenderTarget::eFormat, filter->GetAddressOfTexture()
		);
		ID3D11Helper::CreateUnorderedAccessView(
			DirectXDevice::pDevice,
			filter->GetTexture(),
			filter->GetAddressOfFilterUAV()
		);
	}
}

void FilteredCamera::Resolve()
{
	ID3D11ShaderResourceView** ppInputSRV = nullptr;
	ID3D11Texture2D* pOutputResource = nullptr;

	if (isLinkedWithBackBuffer)
	{
		D3D11_TEXTURE2D_DESC back_buffer_desc;
		DirectXDevice::pBackBuffer->GetDesc(&back_buffer_desc);
		
		if (back_buffer_desc.SampleDesc.Quality != RenderTarget::uiNumQualityLevels ||
			back_buffer_desc.Format != RenderTarget::eFormat
			)
		{
			Apply(RenderTarget::GetAddressOfSRV());
			ppInputSRV = AFilter::GetAddressOfSRV();
			pOutputResource = AFilter::cpTexture2D.Get();
		}
		else
		{
			ppInputSRV = RenderTarget::GetAddressOfSRV();
			pOutputResource = RenderTarget::cpTexture2D.Get();

		}

		if (upFilters.size() > 0)
		{
			for (std::unique_ptr<AFilter>& pFilter : upFilters)
			{
				pFilter->Apply(ppInputSRV);
				ppInputSRV = pFilter->GetAddressOfSRV();
			}

			pOutputResource = upFilters[upFilters.size() - 1]->GetTexture();
		}

		DirectXDevice::pDeviceContext->ResolveSubresource(DirectXDevice::pBackBuffer, 0, pOutputResource, 0, back_buffer_desc.Format);
	}

}

void FilteredCamera::AddBlurState()
{
	upFilters.push_back(std::make_unique<BlurFilter>(
		uiWidth, uiHeight, AFilter::uiArraySize, AFilter::uiNumQualityLevels,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, NULL,
		NULL, D3D11_USAGE_DEFAULT, AFilter::eFormat
	));
}
