#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>
#include <vector>

constexpr size_t ullMaxLightNum = 10;

enum LightType : unsigned int
{
	Directional,
	Point,
	Spot,
	NotALight
};

struct LightSet
{
public:
	LightType			eLightType;
	float				dummy[3];
	DirectX::XMVECTOR	xmvLocation;
	DirectX::XMVECTOR	xmvLightColor;
	DirectX::XMVECTOR	xmvDirection;
	float				fLightStrength;
	float				fFallOffStart;
	float				fFallOffEnd;
	float				fSpotPower;
};

class LightManager
{
public:
	LightManager(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);
	~LightManager();

public:
	static LightSet	sTempLightSet;

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

protected:
	std::vector<LightSet>					vLights;
	unsigned short							usSelectedLightIndex;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpLightsConstantBuffer;

public:
	const unsigned short& GetSelectedLightIndex();
	void SetSelectedLightIndex(const unsigned short& index);

public:
	const std::vector<LightSet>& GetLights();
	void AddDirectionalLight(
		IN const DirectX::XMVECTOR& xmvLocationIn,
		IN const DirectX::XMVECTOR& xmvLightColorIn,
		IN const DirectX::XMVECTOR& xmvDirectionIn
	);
	void AddPointLight();
	void AddSpotLight();

public:
	void Update();

public:
	void SetConstantBuffers();
	void ResetConstantBuffers();
};

