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
	LightManager(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	~LightManager();

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

protected:
	std::vector<class PointLight*>	vPointLights;
	std::vector<class SpotLight*>	vSpotLights;
	size_t							idxSelectedPointLight;
	size_t							idxSelectedSpotLight;

public:
	void Update();

public:
	inline size_t GetSelPointLightIndex() { return idxSelectedPointLight; }
	inline size_t GetSelSpotLightIndex() { return idxSelectedSpotLight; }
	inline size_t& GetSelPointLightIndexRef() { return idxSelectedPointLight; }
	inline size_t& GetSelSpotLightIndexRef() { return idxSelectedSpotLight; }

public:
	inline const std::vector<class PointLight*>& GetPointLights() { return vPointLights; }
	inline const std::vector<class SpotLight*>& GetSpotLights() { return vSpotLights; }

	void AddPointLight(
		IN const DirectX::XMVECTOR& xmvLocationIn,
		IN const DirectX::XMVECTOR& xmvLightColorIn,
		IN const float& fFallOffStart,
		IN const float& fFallOffEnd,
		IN const float& fLightPowerIn
	);
	void AddSpotLight(
		IN const DirectX::XMVECTOR& xmvLocationIn,
		IN const DirectX::XMVECTOR& xmvDirectionIn,
		IN const DirectX::XMVECTOR& xmvLightColorIn,
		IN const float& fFallOffStart,
		IN const float& fFallOffEnd,
		IN const float& fLightPowerIn,
		IN const float& fSpotPower
	);
};

