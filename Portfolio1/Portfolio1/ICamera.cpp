#include "ICamera.h"

ICamera::ICamera(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	IDXGISwapChain* pSwapChainIn,
	UINT uiWidthIn, UINT uiHeightIn, 
	UINT uiArraySizeIn, 
	UINT uiNumQualityLevelsIn, 
	DXGI_FORMAT eRTVFormatIn, 
	DXGI_FORMAT eDSVFormatIn
)
	: pDeviceContext(pDeviceContextIn),
	IRenderTarget(
		pDeviceIn, uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		D3D11_BIND_RENDER_TARGET, NULL, NULL,
		D3D11_USAGE_DEFAULT, eRTVFormatIn
	),
	IDepthStencil(
		pDeviceIn, uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		D3D11_BIND_DEPTH_STENCIL, NULL, NULL,
		D3D11_USAGE_DEFAULT, eDSVFormatIn
	),
	ASwapChainAccessable(
		pDeviceIn,
		pSwapChainIn
	)
{
}

ICamera::~ICamera()
{

}
