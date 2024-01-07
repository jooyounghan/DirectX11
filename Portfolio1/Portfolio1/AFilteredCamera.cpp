#include "AFilteredCamera.h"

AFilteredCamera::AFilteredCamera(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	IDXGISwapChain* pSwapChainIn,
	UINT uiWidthIn, UINT uiHeightIn,
	UINT uiArraySizeIn,
	UINT uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn,
	DXGI_FORMAT eDSVFormatIn
)
	: ICamera(
		pDeviceIn, pDeviceContextIn, pSwapChainIn,
		uiWidthIn, uiHeightIn, 
		uiArraySizeIn, uiNumQualityLevelsIn, 
		eRTVFormatIn, eDSVFormatIn
	)
{
}

AFilteredCamera::~AFilteredCamera()
{
}

void AFilteredCamera::Filter()
{
	if (IsSwapChainAccesssed)
	{
		ID3D11ShaderResourceView** ppInputSRV = cpSRV.GetAddressOf();
		ID3D11Resource* pOutputResource = IRenderTarget::cpTexture2D.Get();
		if (pFilters.size() > 0)
		{
			for (IFilter* pFilter : pFilters)
			{
				pFilter->Apply(ppInputSRV);
				ppInputSRV = pFilter->cpSRV.GetAddressOf();
			}

			pOutputResource = pFilters[pFilters.size() - 1]->cpTexture2D.Get();
		}

		pDeviceContext->ResolveSubresource(ASwapChainAccessable::cpTexture2D.Get(), 0, pOutputResource, 0, ASwapChainAccessable::eFormat);
	}
}
