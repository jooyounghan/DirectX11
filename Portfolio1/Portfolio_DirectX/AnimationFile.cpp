#include "AnimationFile.h"
#include "FileManipulator.h"

using namespace DirectX;

AnimChannelFile::AnimChannelFile(
	const std::string& strChannleNameIn,
	const unsigned int& uiNumPosition,
	const unsigned int& uiNumRotation,
	const unsigned int& uiNumScale,
	void* pPositionIn,
	void* pRotationIn,
	void* pScaleIn
)
	: strAnimChannelData(strChannleNameIn)
{
	vTranslation.resize(uiNumPosition);
	vRotation.resize(uiNumRotation);
	vScale.resize(uiNumScale);

	memcpy(vTranslation.data(), pPositionIn, sizeof(AnimTranslation) * uiNumPosition);
	memcpy(vRotation.data(), pRotationIn, sizeof(AnimRotation) * uiNumRotation);
	memcpy(vScale.data(), pScaleIn, sizeof(AnimScale) * uiNumScale);
}

AnimChannelFile::~AnimChannelFile()
{
}

XMMATRIX AnimChannelFile::GetChannelTransformation(const double& dblPlayTimeIn)
{
	XMVECTOR xmvTranslation = GetInterpolatedData(dblPlayTimeIn, vTranslation, 0.f, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMVECTOR xmvQuaternion = GetInterpolatedData(dblPlayTimeIn, vRotation, 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	XMVECTOR xmvScaling = GetInterpolatedData(dblPlayTimeIn, vScale, 1.f, XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return XMMatrixTranslationFromVector(xmvTranslation) * XMMatrixRotationQuaternion(xmvQuaternion) * XMMatrixScalingFromVector(xmvScaling);
}


AnimationFile::AnimationFile(
	const std::string& strFileLabelIn,
	const double& dblDurationIn,
	const double& dblTicksPerSecondIn
)
	: IFile(strFileLabelIn), dblDuration(dblDurationIn), dblTicksPerSecond(dblTicksPerSecondIn)
{
}

AnimationFile::~AnimationFile()
{
}

void AnimationFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_ANIMATION_KEY);
}
