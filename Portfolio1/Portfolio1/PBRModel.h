#pragma once
#include "PickableModelInterface.h"
#include "ShaderTypeEnum.h"
#include "ModelTextureFile.h"

class PBRModel : public PickableModelInterface
{
public:
	PBRModel(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~PBRModel();

#pragma region Virtual
// Virtual Function ==============================================
public:
	virtual void Update(const float& fDelta);

public:
	virtual void SetIAProperties() override;

public:
	virtual void SetConstantBuffers() override;
	virtual void ResetConstantBuffers() override;

public:
	virtual void SetShaderResources() override;
	virtual void ResetShaderResources() override;
// ==============================================================

public:
	std::shared_ptr<class ModelTextureFile>	pModelTextureSet[TEXTURE_MAP_NUM];
	std::shared_ptr<class ModelTextureFile>	pHeightTexture;

public:
	struct
	{
		uint32_t bIsTextureOn[TEXTURE_MAP_NUM];
		uint32_t uiDummy[TEXTURE_MAP_NUM % 4];
	} sPSTextureFlags;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTextureFlagBuffer;

public:
	struct
	{
		float	fFrenelConstant[3];
		uint32_t uiDummy;
	} sPSTextureConstants;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTextureConstantBuffer;
};

