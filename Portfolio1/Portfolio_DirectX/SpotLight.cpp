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
	: ILight(fXPos, fYPos, fZPos),
	ViewableDepthStencil(
		fXPos, fYPos, fZPos, 
		DirectX::XMConvertToRadians(90.f), 
		sBaseLightData.fFallOffStart, 
		sBaseLightData.fFallOffEnd,
		uiWidthIn, uiHeightIn,
		0, DXGI_FORMAT_R32_TYPELESS
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		(float)uiWidthIn, 
		(float)uiHeightIn, 
		DirectX::XMConvertToRadians(90.f),
		sBaseLightData.fFallOffStart, 
		sBaseLightData.fFallOffEnd
	),
	IMovable(fXPos, fYPos, fZPos)
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


void SpotLight::UpdateLight()
{

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sBaseLightData, D3D11_MAP_WRITE_DISCARD,
		cpBaseLightBuffer.Get()
	);

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sSpotLightData, D3D11_MAP_WRITE_DISCARD, 
		cpSpotLightBuffer.Get()
	);
}