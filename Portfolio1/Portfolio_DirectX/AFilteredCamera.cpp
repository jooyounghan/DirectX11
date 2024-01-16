#include "AFilteredCamera.h"
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
	: ICamera(
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

		DirectXDevice::pDeviceContext->ResolveSubresource(ASwapChainAccessable::cpTexture2D.Get(), 0, pOutputResource, 0, ASwapChainAccessable::eFormat);
	}
}
