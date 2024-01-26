#include "ILight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ILight::ILight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos
)
	: IMovable(fXPos, fYPos, fZPos)
{
	AutoZeroMemory(sBaseLightData);

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
