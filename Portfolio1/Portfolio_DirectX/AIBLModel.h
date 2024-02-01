#pragma once
#include "AStaticMesh.h"

#include <memory>

class AIBLModel : public AStaticMesh
{
public:
	AIBLModel();
	virtual ~AIBLModel();

public:
	std::shared_ptr<class NormalImageFile>	spIBLTextureFile;
	std::shared_ptr<class DDSImageFile>		spEnvSpecularTextureFile;
	std::shared_ptr<class DDSImageFile>		spEnvDiffuseTextureFile;
	std::shared_ptr<class NormalImageFile>	spEnvBrdfTextureFile;

private:
	virtual void Load(const std::string& path) override {};

public:
	virtual void UpdateModel(const float& fDelta) = 0;

public:
	virtual void AcceptModelManipulator(class ModelManipulator* pModelManipulator);
};

