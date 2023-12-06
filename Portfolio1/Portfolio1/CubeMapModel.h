#pragma once
#include "ModelInterface.h"

class CubeMapModel : public ModelInterface
{
public:
	CubeMapModel(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);

	virtual ~CubeMapModel();

#pragma region Virtual
// Virtual Function ==============================================
public:
	virtual void Update(const float& fDelta);
	virtual void Render() override;

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
	std::shared_ptr<class DDSFile>				spEnvSpecularTextureFile;
	std::shared_ptr<class DDSFile>				spEnvDiffuseTextureFile;
	std::shared_ptr<class ModelTextureFile>		spEnvBrdfTextureFile;

public:
	struct
	{
		uint32_t bIsSpecularTextureOn;
		uint32_t bIsDiffuseTextureOn;
		uint32_t bIsBrdfTextureOn;
		uint32_t uiDummy[1];
	} sPSTextureFlags;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTextureFlagBuffer;
};

