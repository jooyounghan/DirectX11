#pragma once
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>
#include "ModelStruct.h"

class ModelInterface
{
public:
	ModelInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);
	virtual ~ModelInterface();

public:
	virtual void Update(const float& fDelta) = 0;

public:
	virtual void Render() = 0;

public:
	virtual void SetIAProperties() = 0;
	virtual void SetVSConstantBuffers() = 0;
	virtual void SetHSConstantBuffers() = 0;
	virtual void SetDSConstantBuffers() = 0;
	virtual void SetGSConstantBuffers() = 0;
	virtual void SetPSConstantBuffers() = 0;
	virtual void ResetConstantBuffers() = 0;

public:
	virtual void SetVSShaderResources() = 0;
	virtual void SetHSShaderResources() = 0;
	virtual void SetDSShaderResources() = 0;
	virtual void SetGSShaderResources() = 0;
	virtual void SetPSShaderResources() = 0;
	virtual void ResetShaderResources() = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>&			cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>&	cpDeviceContext;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t								ui32IndexCount;

protected:
	static void MakePlaneVertexIndexSet(
		ModelInterface* pModelInterface,
		DirectX::XMVECTOR& xmvDirection,
		DirectX::XMVECTOR& xmvUp,
		const float& fWidth,
		const float& fHeight
	);

	static void MakeSquareVertexIndexSet(
		ModelInterface* pModelInterface,
		const float& fLen,
		const bool& bReverse = false
	);
	static void MakeSphereVertexIndexSet(
		ModelInterface* pModelInterface,
		const float& fRadius,
		const unsigned short& usLevel
	);
};

