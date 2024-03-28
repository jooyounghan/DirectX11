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
	// �ð��� ���� ������Ʈ ���� �ش� �ð��� ���� ���ؽ� ������Ʈ ����
}

void SkeletalModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}
