#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

enum ELightType : uint32_t
{
	PointLight = 1,
	SpotLight
};

class LightInterface
{
public:
	LightInterface(
		ID3D11Device* pDeviceIn, 
		ID3D11DeviceContext* pDeviceContextIn
	);
	~LightInterface();

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

public:
	struct
	{
		uint32_t	uiLightType;
		float		fLightColor[3];
		float		fLightPower;
		float		fDummy[3];
	} sBaseLightData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpBaseLightDataBuffer;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpShadowMapTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpShadowMapSRV;

public:
	virtual void Update();

public:
	virtual void SetConstantBuffers() = 0;
	virtual void ResetConstantBuffers() = 0;

public:
	virtual void SetShaderResources() = 0;
	virtual void ResetShaderResources() = 0;
};

