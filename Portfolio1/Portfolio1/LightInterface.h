#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

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
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpBaseLightDataBuffer;

public:
	virtual void Update() = 0;

public:
	virtual void SetConstantBuffers() = 0;
	virtual void ResetConstantBuffers() = 0;

public:
	virtual void SetShaderResources() = 0;
	virtual void ResetShaderResources() = 0;
};

