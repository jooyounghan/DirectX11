#pragma once
#include "ModelInterface.h"

class PickableModel : public ModelInterface
{
public:
	PickableModel(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);
	virtual ~PickableModel();

public:
	virtual void Update(const float& fDelta);
	virtual void Render() override;

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
	std::unique_ptr<class ModelID>			upModelID;

public:
	std::unique_ptr<class TransformProperties >			upTransformationProperties;
	std::unique_ptr<class TransformationBufferData>		upTransformationBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				cpTransformationDataBuffer;

public:
	void ScaleUp(const float& x, const float& y, const float& z);

protected:
	static void MakePlaneVertexIndexSet(
		PickableModel* pModelInterface,
		DirectX::XMVECTOR& xmvDirection,
		DirectX::XMVECTOR& xmvUp,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fWidth,
		const float& fHeight
	);

	static void MakeSquareVertexIndexSet(
		PickableModel* pModelInterface,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fLen,
		const bool& bReverse = false
	);
	static void MakeSphereVertexIndexSet(
		PickableModel* pModelInterface,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fRadius,
		const unsigned short& usLevel
	);
};

