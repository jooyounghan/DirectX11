#include "ILight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ILight::ILight(
	const float& fXPosIn,
	const float& fYPosIn,
	const float& fZPosIn,
	const float& fLightRColorIn,
	const float& fLightGColorIn,
	const float& fLightBColorIn,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const float& fLightPowerIn
)
	: IMovable(fXPosIn, fYPosIn, fZPosIn)
{
	AutoZeroMemory(sBaseLightData);

	sBaseLightData.fLightColor[0] = fLightRColorIn;
	sBaseLightData.fLightColor[1] = fLightGColorIn;
	sBaseLightData.fLightColor[2] = fLightBColorIn;

	sBaseLightData.fFallOffStart = fFallOffStartIn;
	sBaseLightData.fFallOffEnd = fFallOffEndIn;
	sBaseLightData.fLightPower = fLightPowerIn;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice,
		sBaseLightData, 
		D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, 
		NULL, 
		cpBaseLightBuffer.GetAddressOf()
	);
}

ILight::~ILight()
{
}
