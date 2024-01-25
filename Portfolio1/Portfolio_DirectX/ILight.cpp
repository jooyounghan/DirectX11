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
	AutoZeroMemory(sLightBase);

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sLightBase, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL, cpLightBaseBuffer.GetAddressOf()
	);
}

ILight::~ILight()
{
}
