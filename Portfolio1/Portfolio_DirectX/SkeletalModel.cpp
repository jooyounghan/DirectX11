#include "SkeletalModel.h"

SkeletalModel::SkeletalModel(const std::shared_ptr<ModelFile>& spModelFile)
	: GroupPBRModel(spModelFile), 
	IMovable(0.f, 0.f, 0.f),
	IScalable(),
	IAngleAdjustable(0.f, 0.f, 0.f)
{
}

SkeletalModel::~SkeletalModel()
{
}
