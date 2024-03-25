#pragma once
#include "GroupPBRModel.h"
#include "SkeletalFile.h"
#include "ModelFile.h"

class SkeletalModel : public GroupPBRModel
{
public:
	SkeletalModel(const std::shared_ptr<ModelFile>& spModelFile);
	virtual ~SkeletalModel();

private:
	std::shared_ptr<SkeletalFile> spSkeletalFile;
};

