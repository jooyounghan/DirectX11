#pragma once
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
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
	virtual void Update();
	void Render();

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

public:
	void ScaleUp(const float& x, const float& y, const float& z);

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>&			cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>&	cpDeviceContext;

public:
	std::unique_ptr<class TransformProperties >			upTransformationProperties;
	std::unique_ptr<class TransformationBufferData>		upTransformationBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				cpTransformationDataBuffer;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t								ui32IndexCount;

protected:
	static void MakeSquareVertexIndexSet(
		ModelInterface* pModelInterface,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fLen,
		const bool& bReverse = false
	);
	static void MakeSphereVertexIndexSet(
		ModelInterface* pModelInterface,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fRadius,
		const unsigned short& usLevel
	);
};

