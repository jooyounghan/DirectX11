#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

SpotLight::SpotLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchRadIn,
	const float& fYawRadIn,
	const float& fRollRadIn
)
	: ILight(fXPos, fYPos, fZPos),
	ViewableDepthOnly(
		fXPos, fYPos, fZPos, 
		fPitchRadIn, fYawRadIn, fRollRadIn,
		DirectX::XMConvertToRadians(90.f), 
		sBaseLightData.fFallOffStart, 
		sBaseLightData.fFallOffEnd,
		1000, 1000
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		fPitchRadIn, fYawRadIn, fRollRadIn,
		1000.f, 
		1000.f, 
		DirectX::XMConvertToRadians(90.f),
		sBaseLightData.fFallOffStart, 
		sBaseLightData.fFallOffEnd
	),
	IAngleAdjustable(fPitchRadIn, fYawRadIn, fRollRadIn),
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(1000, 1000)
{
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice,
		sSpotLightData, D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE,
		NULL, cpSpotLightBuffer.GetAddressOf()
	);
}

SpotLight::~SpotLight()
{

}

void SpotLight::UpdateLight()
{
	fNearZ = sBaseLightData.fFallOffStart;
	fFarZ = sBaseLightData.fFallOffEnd;

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