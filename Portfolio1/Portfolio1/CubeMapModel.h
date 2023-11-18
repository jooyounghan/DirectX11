#pragma once
#include "ModelInterface.h"

class CubeMapModel : public ModelInterface
{
public:
	CubeMapModel(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);

	virtual ~CubeMapModel();

public:
	virtual void Update();

public:
	virtual void SetIAProperties() override;
	virtual void SetVSConstantBuffers() override;
	virtual void SetHSConstantBuffers() override;
	virtual void SetDSConstantBuffers() override;
	virtual void SetGSConstantBuffers() override;
	virtual void SetPSConstantBuffers() override;
	virtual void ResetConstantBuffers() override;

public:
	virtual void SetVSShaderResources() override;
	virtual void SetHSShaderResources() override;
	virtual void SetDSShaderResources() override;
	virtual void SetGSShaderResources() override;
	virtual void SetPSShaderResources() override;
	virtual void ResetShaderResources() override;

public:
	std::shared_ptr<class DDSFile>				spEnvSpecularTextureFile;
	std::shared_ptr<class DDSFile>				spEnvIrradianceTextureFile;
	std::shared_ptr<class ModelTextureFile>		spEnvBrdfTextureFile;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTextureFlagBuffer;
	struct
	{
		uint32_t bIsSpecularTextureOn;
		uint32_t bIsIrradianceTextureOn;
		uint32_t bIsBrdfTextureOn;
		uint32_t uiDummy[1];
	} sPSTextureFlags;
};

