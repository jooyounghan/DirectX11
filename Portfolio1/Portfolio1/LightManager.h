#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>
#include <vector>

class LightManager
{
public:
	LightManager(ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	~LightManager();

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

protected:
	std::vector<class LightInterface*>		vLights;
	size_t									idxSelectedLight;

public:
	void Update();

public:
	const size_t& GetSelectedLightIndex();
	void SetSelectedLightIndex(const size_t& index);

public:
	inline const std::vector<class LightInterface*>& GetLights() { return vLights; }

	void AddPointLight(
		IN const DirectX::XMVECTOR& xmvLocationIn,
		IN const float* pLightColorIn,
		IN const float& fFallOffStart,
		IN const float& fFallOffEnd,
		IN const float& fLightPowerIn
	);
	void AddSpotLight(
		IN const DirectX::XMVECTOR& xmvLocationIn,
		IN const DirectX::XMVECTOR& xmvDirectionIn,
		IN const float* pLightColorIn,
		IN const float& fFallOffStart,
		IN const float& fFallOffEnd,
		IN const float& fLightPowerIn,
		IN const float& fSpotPower
	);
};

