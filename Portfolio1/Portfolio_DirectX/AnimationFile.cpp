#include "AnimationFile.h"
#include "FileManipulator.h"

AnimChannelFile::AnimChannelFile(
	const std::string& strChannleNameIn,
	const unsigned int& uiNumPosition,
	void* pPositionIn,
	const unsigned int& uiNumRotation,
	void* pRotationIn,
	const unsigned int& uiNumScale,
	void* pScaleIn
)
	: strAnimChannelData(strChannleNameIn)
{
	vTransformation.resize(uiNumPosition);
	memcpy(vTransformation.data(), pPositionIn, sizeof(AnimTransformation) * uiNumPosition);
	vRotation.resize(uiNumRotation);
	memcpy(vRotation.data(), pRotationIn, sizeof(AnimRotation) * uiNumRotation);
	vScale.resize(uiNumScale);
	memcpy(vScale.data(), pScaleIn, sizeof(AnimScale) * uiNumScale);
}

AnimChannelFile::~AnimChannelFile()
{
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
