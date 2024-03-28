#pragma once
#include "GroupPBRModel.h"
#include "SkeletalModelFile.h"
#include "Animation.h"

class SkeletalModel : public GroupPBRModel
{
public:
	SkeletalModel(const std::shared_ptr<SkeletalModelFile>& spSkeletalModelFile);
	virtual ~SkeletalModel();

private:
	std::shared_ptr<BoneFile>		spBoneFile;
	
private:
	Animation animation;

public:
	Bone* GetBone() { return spBoneFile.get(); }

public:
	virtual void UpdateModel(const float& fDelta) override;

public:
	inline void SetAnimation(const std::shared_ptr<AnimationFile>& spAnimFileIn) { animation = Animation(spAnimFileIn); }
	inline Animation& GetAnimation() { return animation; }
	inline bool IsAnimationValid() { return animation.GetAnimationFile() != nullptr; }

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
};

