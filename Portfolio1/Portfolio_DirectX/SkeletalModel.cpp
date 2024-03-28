#include "SkeletalModel.h"
#include "ModelManipulator.h"

SkeletalModel::SkeletalModel(const std::shared_ptr<SkeletalModelFile>& spSkeletalModelFile)
	: GroupPBRModel(spSkeletalModelFile), animation(nullptr),
	IMovable(0.f, 0.f, 0.f),
	IScalable(),
	IAngleAdjustable(0.f, 0.f, 0.f)
{
	spBoneFile = spSkeletalModelFile->GetBoneFile();
}

SkeletalModel::~SkeletalModel()
{
}

void SkeletalModel::UpdateModel(const float& fDelta)
{
	// 시간에 대한 업데이트 이후 해당 시간에 대한 버텍스 업데이트 수행
}

void SkeletalModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}
