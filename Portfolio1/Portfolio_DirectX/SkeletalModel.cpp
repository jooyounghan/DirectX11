#include "SkeletalModel.h"
#include "ModelManipulator.h"

SkeletalModel::SkeletalModel(const std::shared_ptr<SkeletalModelFile>& spSkeletalModelFile)
	: GroupPBRModel(spSkeletalModelFile),
	IMovable(0.f, 0.f, 0.f),
	IScalable(),
	IAngleAdjustable(0.f, 0.f, 0.f)
{
	spBoneFile = spSkeletalModelFile->GetBoneFile();
}

SkeletalModel::~SkeletalModel()
{
}

void SkeletalModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}
