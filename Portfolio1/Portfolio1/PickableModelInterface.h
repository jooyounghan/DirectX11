#pragma once
#include "ModelInterface.h"

class PickableModelInterface : public ModelInterface
{
public:
	PickableModelInterface(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~PickableModelInterface();

public:
	std::unique_ptr<class ModelID>			upModelID;

public:
	struct
	{
		struct { float fX, fY, fZ; }			sScales;
		struct { float fPitch, fYaw, fRoll; }	sAngles;
		struct { float fX, fY, fZ; }			sTranslations;
	} sTransformation;

public:
	struct
	{
		DirectX::XMMATRIX xmmTransformationMatrix;
		DirectX::XMMATRIX xmmInvTranformationMatrix;
	} sTranformationData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		cpTransformationBuffer;

#pragma region Virtual
// Virtual Function ==============================================
public:
	virtual void Update(const float& fDelta);
	virtual void Render() override;
	virtual void ScaleUp(const float& x, const float& y, const float& z);
// ==============================================================
#pragma endregion

#pragma region Interface
// Interface Function ============================================
public:
	virtual void SetIAProperties() = 0;

public:
	virtual void SetConstantBuffers() = 0;
	virtual void ResetConstantBuffers() = 0;

public:
	virtual void SetShaderResources() = 0;
	virtual void ResetShaderResources() = 0;
// ==============================================================
#pragma endregion

protected:
	static void MakePlaneVertexIndexSet(
		PickableModelInterface* pModelInterface,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fWidth,
		const float& fHeight,
		const float& fWrapWidth,
		const float& fWrapHeight
	);

	static void MakeSquareVertexIndexSet(
		PickableModelInterface* pModelInterface,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fLen,
		const bool& bReverse = false
	);
	static void MakeSphereVertexIndexSet(
		PickableModelInterface* pModelInterface,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fRadius,
		const unsigned short& usLevel
	);
};

