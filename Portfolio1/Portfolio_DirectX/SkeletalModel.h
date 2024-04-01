#pragma once
#include "GroupPBRModel.h"
#include "SkeletalModelFile.h"
#include "AnimationFile.h"

class SkeletalModel : public GroupPBRModel
{
public:
	SkeletalModel(const std::shared_ptr<SkeletalModelFile>& spSkeletalModelFile);
	virtual ~SkeletalModel();

private:
	std::shared_ptr<BoneFile>		spBoneFile;
	std::shared_ptr<AnimationFile>	spAnimFile;

private:
	DirectX::XMMATRIX xmmRootTransform;
	DirectX::XMVECTOR xmvPreviousTranslation;

public:
	Bone* GetBone() { return spBoneFile.get(); }
	AnimationFile* GetAnimationFile() { return spAnimFile.get(); }

public:
	inline void SetAnimationFile(const std::shared_ptr<AnimationFile>& spAnimFileIn) { spAnimFile = spAnimFileIn; }

private:
	double dblAnimPlayTime;
	struct
	{
		double dblAnimSpeed;
		double dblDummy;
	} sAnimData;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpAnimationBuffer;

public:
	void SetAnimSpeed(const double& dblSpeed) { sAnimData.dblAnimSpeed = dblSpeed; }
	const double& GetAnimSpeed() { return sAnimData.dblAnimSpeed; }
	double* GetAnimSpeedAddress() { return &sAnimData.dblAnimSpeed; }

public:
	void AddPlayTime(const float& fDeltaTime);
	const double& GetPlayTime() { return dblAnimPlayTime; }

private:
	std::unordered_map<std::string, DirectX::XMMATRIX> skeletalTransformation;

public:
	virtual void UpdateModel(const float& fDelta) override;
	virtual void UpdateBoneTransformation(Bone& bone);

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
};

