#pragma once
#include "AStaticMesh.h"
#include "DDSImageFile.h"
#include "NormalImageFile.h"

#include <memory>

class AIBLModel : public AStaticMesh
{
public:
	AIBLModel();
	virtual ~AIBLModel();

public:
	struct
	{
		float DiffuseRate;
		float fDummy[3];
	} sIBLData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpIBLDataBuffer;

public:
	std::shared_ptr<class DDSImageFile>		spEnvSpecularTextureFile;
	std::shared_ptr<class DDSImageFile>		spEnvDiffuseTextureFile;
	std::shared_ptr<class NormalImageFile>	spEnvBrdfTextureFile;

private:
	virtual void Load(const std::string& path) override {};

public:
	virtual void UpdateModel(const float& fDelta) override;

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;

private:
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override {};
};

