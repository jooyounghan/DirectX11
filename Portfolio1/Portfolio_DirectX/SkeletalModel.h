#pragma once
#include "GroupPBRModel.h"
#include "SkeletalModelFile.h"

class SkeletalModel : public GroupPBRModel
{
public:
	SkeletalModel(const std::shared_ptr<SkeletalModelFile>& spSkeletalModelFile);
	virtual ~SkeletalModel();

private:
	std::shared_ptr<BoneFile> spBoneFile;

public:
	Bone* GetBone() { return spBoneFile.get(); }

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
};

