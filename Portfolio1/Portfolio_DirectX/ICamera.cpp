#include "ICamera.h"

ICamera::ICamera(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	IDXGISwapChain* pSwapChainIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovRadIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn, 
	DXGI_FORMAT eDSVFormatIn
)
	: IRenderTarget(
		pDeviceIn, pDeviceContextIn, uiWidthIn, uiHeightIn,
		1, uiNumQualityLevelsIn,
		D3D11_BIND_RENDER_TARGET, NULL, NULL,
		D3D11_USAGE_DEFAULT, eRTVFormatIn
	),
	IDepthStencil(
		pDeviceIn, pDeviceContextIn, uiWidthIn, uiHeightIn,
		1, uiNumQualityLevelsIn,
		D3D11_BIND_DEPTH_STENCIL, NULL, NULL,
		D3D11_USAGE_DEFAULT, eDSVFormatIn
	),
	ASwapChainAccessable(
		pDeviceIn,
		pDeviceContextIn,
		pSwapChainIn
	),
	AViewable(
		pDeviceIn,
		pDeviceContextIn, 
		uiWidthIn, uiHeightIn, fFovRadIn,
		fNearZIn, fFarZIn
	),
	IDirectXDevice(
		pDeviceIn,
		pDeviceContext
	),
	IRectangle(
		uiWidthIn,
		uiHeightIn,
		1,
		uiNumQualityLevelsIn
	)
{
}

ICamera::~ICamera()
{

}
