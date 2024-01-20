#include "ICamera.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"

ICamera::ICamera(
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
	: IRenderTarget(
		uiWidthIn, uiHeightIn,
		1, uiNumQualityLevelsIn,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT, eRTVFormatIn
	),
	IDepthStencil(
		uiWidthIn, uiHeightIn,
		1, uiNumQualityLevelsIn,
		D3D11_BIND_DEPTH_STENCIL, NULL, NULL,
		D3D11_USAGE_DEFAULT, eDSVFormatIn
	),
	ASwapChainAccessable(),
	AViewable(
		fXPos, fYPos, fZPos,
		(float)uiWidthIn, (float)uiHeightIn, fFovRadIn,
		fNearZIn, fFarZIn
	),
	IRectangle(
		uiWidthIn,
		uiHeightIn
	)
{
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, 
		sTexelSize, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpTexelSize.GetAddressOf()
	);
}

ICamera::~ICamera()
{

}
