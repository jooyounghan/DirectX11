#pragma once
#include "AnimationFile.h"

class Animation
{
public:
	Animation(const std::shared_ptr<AnimationFile>& spAnimationFileIn);
	~Animation();

private:
	std::shared_ptr<AnimationFile> spAnimFile;

public:
	AnimationFile* GetAnimationFile() { return spAnimFile.get(); }

private:
	double dblAnimPlayTime;

public:
	inline const double& GetAnimPlayTime() { return dblAnimPlayTime; };
	inline void SetAnimPlayTime(const double& animPlayTimeIn) { dblAnimPlayTime = animPlayTimeIn; }

	struct
	{
		double dblAnimSpeed;
		double dblDummy;
	} sAnimData;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpAnimationBuffer;
	
public:
	void UpdateAnimationBuffer();

public:
	void SetAnimSpeed(const double& dblSpeed) { sAnimData.dblAnimSpeed = dblSpeed; }
	const double& GetAnimSpeed(const double& dblSpeed) { return sAnimData.dblAnimSpeed; }
	double* GetAnimSpeedAddress() { return &sAnimData.dblAnimSpeed; }
};

