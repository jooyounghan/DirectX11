#include "AnimationFile.h"
#include "FileManipulator.h"
#include <algorithm>

using namespace std;
using namespace DirectX;

AnimChannel::AnimChannel()
{

}

AnimChannel::~AnimChannel()
{
}

DirectX::XMVECTOR AnimChannel::GetTranslation(const double& dblPlayTimeIn)
{
	return GetInterpolatedData(dblPlayTimeIn, vTranslation, 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f));
}

XMMATRIX AnimChannel::GetTransformation(const double& dblPlayTimeIn)
{
	XMVECTOR xmvTranslation = GetInterpolatedData(dblPlayTimeIn, vTranslation, 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	XMVECTOR xmvQuaternion = GetInterpolatedData(dblPlayTimeIn, vRotation, 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	XMVECTOR xmvScaling = GetInterpolatedData(dblPlayTimeIn, vScale, 1.f, XMVectorSet(1.f, 1.f, 1.f, 0.f));

	return XMMatrixScalingFromVector(xmvScaling)
		* XMMatrixRotationQuaternion(xmvQuaternion)
		* XMMatrixTranslationFromVector(xmvTranslation);
}

DirectX::XMMATRIX AnimChannel::GetTransformationWithoutTranslation(const double& dblPlayTimeIn)
{
	XMVECTOR xmvQuaternion = GetInterpolatedData(dblPlayTimeIn, vRotation, 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	XMVECTOR xmvScaling = GetInterpolatedData(dblPlayTimeIn, vScale, 1.f, XMVectorSet(1.f, 1.f, 1.f, 0.f));

	return XMMatrixScalingFromVector(xmvScaling)
		* XMMatrixRotationQuaternion(xmvQuaternion)
		* XMMatrixTranslationFromVector(XMVectorSet(0.f, 0.f, 0.f, 1.f));
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
