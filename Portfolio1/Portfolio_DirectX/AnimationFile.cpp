#include "AnimationFile.h"
#include "FileManipulator.h"

using namespace DirectX;

AnimChannel::AnimChannel()
{

}

AnimChannel::~AnimChannel()
{
}

XMMATRIX AnimChannel::GetChannelTransformation(const double& dblPlayTimeIn)
{
	XMVECTOR xmvTranslation = GetInterpolatedData(dblPlayTimeIn, vTranslation, 0.f, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMVECTOR xmvQuaternion = GetInterpolatedData(dblPlayTimeIn, vRotation, 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	XMVECTOR xmvScaling = GetInterpolatedData(dblPlayTimeIn, vScale, 1.f, XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return XMMatrixTranslationFromVector(xmvTranslation) * XMMatrixRotationQuaternion(xmvQuaternion) * XMMatrixScalingFromVector(xmvScaling);
}


AnimationFile::AnimationFile(
	const std::string& strFileLabelIn,
	const std::shared_ptr<BoneFile>& spBoneFileIn,
	const double& dblDurationIn,
	const double& dblTicksPerSecondIn
)
	: IFile(strFileLabelIn), dblDuration(dblDurationIn), dblTicksPerSecond(dblTicksPerSecondIn)
{
	vAnimChannels.resize(spBoneFileIn->GetBoneNums());
}

AnimationFile::~AnimationFile()
{
}

void AnimationFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_ANIMATION_KEY);
}
