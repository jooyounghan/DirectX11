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
	std::shared_ptr<class DDSImageFile>		spEnvSpecularTextureFile;
	std::shared_ptr<class DDSImageFile>		spEnvDiffuseTextureFile;
	std::shared_ptr<class NormalImageFile>	spEnvBrdfTextureFile;

private:
	virtual void Load(const std::string& path) override {};

public:
	virtual void AcceptModelManipulator(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRenderer(class ModelRenderer* pModelRenderer) override;
};

