#pragma once

#include "ILight.h"

class DirectionalLight : public ILight
{
public:
	DirectionalLight(
		IN Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		IN Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		IN const DirectX::XMVECTOR& xmvLocationIn,
		IN const DirectX::XMVECTOR& xmvLightColorIn,
		IN const DirectX::XMVECTOR& xmvDirectionIn
	);
	~DirectionalLight();
};

