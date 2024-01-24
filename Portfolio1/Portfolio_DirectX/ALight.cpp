#include "ALight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ALight::ALight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fWidthIn,
	const float& fHeightIn
)
	: AViewable(
		fXPos, fYPos, fZPos, 
		fWidthIn, fHeightIn,
		DirectX::XMConvertToRadians(90.f), 0.f, 1.f
	)
{
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sLightBase, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL, cpLightBaseBuffer.GetAddressOf()
	);
}

ALight::~ALight()
{
}

void ALight::UpdateLight()
{
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sLightBase, D3D11_MAP_WRITE_DISCARD,
		cpLightBaseBuffer.Get()
	);
}
