#pragma once
#include "PBRStaticMesh.h"
#include "BoneFile.h"
#include "AnimationFile.h"

class SkeletalModel : public PBRStaticMesh
{
public:
	SkeletalModel(
		const std::shared_ptr<MeshFile>& spMeshFileIn
	);
	virtual ~SkeletalModel();

private:
	std::shared_ptr<AnimationFile>	spAnimFile;

private:
	DirectX::XMMATRIX xmmRootTransform;
	DirectX::XMVECTOR xmvPreviousTranslation;

public:
	inline void SetAnimationFile(const std::shared_ptr<AnimationFile>& spAnimFileIn) { spAnimFile = spAnimFileIn; }
	AnimationFile* GetAnimationFile() { return spAnimFile.get(); }

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
	std::vector<DirectX::XMMATRIX> vBoneTransformation;

public:
	virtual void UpdateModel(const float& fDelta) override;
	//virtual void UpdateBoneTransformation(Bone& bone);

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
};

