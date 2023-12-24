#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

struct LightSet
{
	uint32_t			uiLightType;
	DirectX::XMVECTOR	xmvLocation;
	DirectX::XMVECTOR	xmvDirect;
	float				fLightColor[3];
	float				fFallOffStart;
	float				fFallOffEnd;
	float				fLightPower;
	float				fSpotPower;
};

enum ELightType : size_t
{
	PointLightType = 1,
	SpotLightType
};

class LightInterface
{
public:
	LightInterface(
		ID3D11Device* pDeviceIn, 
		ID3D11DeviceContext* pDeviceContextIn,
		const DirectX::XMVECTOR& xmvLocationIn,
		const float* pLightColorIn,
		const float& fLightPowerIn
	);
	~LightInterface();

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

public:
	LightSet sBaseLightData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpBaseLightDataBuffer;

public:
	virtual void Update();

public:
	virtual void SetConstantBuffers() = 0;
	virtual void ResetConstantBuffers() = 0;

public:
	virtual void SetShaderResources() = 0;
	virtual void ResetShaderResources() = 0;
};

