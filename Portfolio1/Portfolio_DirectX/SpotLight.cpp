#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

SpotLight::SpotLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const UINT& uiWidthIn,
	const UINT& uiHeightIn
)
	: ALight(fXPos, fYPos, fZPos, uiWidthIn, uiHeightIn),
	ARenderTarget(
		uiWidthIn, uiHeightIn, 1, 0,
		D3D11_BIND_RENDER_TARGET,
		NULL, NULL,
		D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R8_UINT
	),
	ADepthStencil(
		uiWidthIn, uiHeightIn, 1, 0,
		D3D11_BIND_DEPTH_STENCIL,
		NULL, NULL,
		D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_D24_UNORM_S8_UINT
	),
	ARectangle(uiWidthIn, uiHeightIn)
{
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice,
		sSpotLightData, D3D11_USAGE_DYNAMIC, 
		NULL, D3D11_CPU_ACCESS_WRITE,
		NULL, cpSpotLightBuffer.GetAddressOf()
	);
}

SpotLight::~SpotLight()
{

}

void SpotLight::ClearRTV()
{
	DirectXDevice::pDeviceContext->ClearRenderTargetView(
		ARenderTarget::cpRTV.Get(),
		ARenderTarget::fClearColor
	);
}

void SpotLight::ClearDSV()
{
	DirectXDevice::pDeviceContext->ClearDepthStencilView(
		ADepthStencil::cpDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f, 0
	);
}

void SpotLight::UpdateLight()
{
	ALight::UpdateLight();
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sSpotLightData, D3D11_MAP_WRITE_DISCARD, 
		cpSpotLightBuffer.Get()
	);
}

void SpotLight::Resize(
	const UINT& uiWidthIn, 
	const UINT& uiHeightIn
)
{
	SetRectangle(uiWidthIn, uiHeightIn);
	ARenderTarget::Resize(uiWidthIn, uiHeightIn);
	ADepthStencil::Resize(uiWidthIn, uiHeightIn);
}