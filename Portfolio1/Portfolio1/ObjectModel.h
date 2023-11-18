#pragma once
#include "ModelInterface.h"
#include "ShaderTypeEnum.h"
#include "ModelTextureFile.h"

class ObjectModel : public ModelInterface
{
public:
	ObjectModel(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);
	virtual ~ObjectModel();

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
	std::shared_ptr<class ModelTextureFile>	pModelTextureSet[TEXTURE_MAP_NUM];
	std::shared_ptr<class ModelTextureFile>	pHeightTexture;

public:
	std::unique_ptr<class ModelID>			upModelID;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTextureFlagBuffer;
	struct
	{
		uint32_t bIsTextureOn[TEXTURE_MAP_NUM];
		uint32_t uiDummy[2];
	} sPSTextureFlags;

};

