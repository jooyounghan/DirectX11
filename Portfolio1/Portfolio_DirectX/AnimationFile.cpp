#include "AnimationFile.h"
#include "FileManipulator.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace DirectX;

AnimChannel::AnimChannel()
{

}

AnimChannel::~AnimChannel()
{
}


void AnimChannel::AddTranslation(const double& dblTimeIn, const float& x, const float& y, const float& z)
{
	vTranslation.emplace_back();
	AnimTranslation& current_translation = vTranslation[vTranslation.size() - 1];
	current_translation.dblTime = dblTimeIn;
	current_translation.xmElement.x = x;
	current_translation.xmElement.y = y;
	current_translation.xmElement.z = z;
}

void AnimChannel::AddRotationQuat(const double& dblTimeIn, const float& x, const float& y, const float& z, const float& w)
{
	vRotation.emplace_back();
	AnimRotation& current_rotation = vRotation[vRotation.size() - 1];
	current_rotation.dblTime = dblTimeIn;
	current_rotation.xmElement.x = x;
	current_rotation.xmElement.y = y;
	current_rotation.xmElement.z = z;
	current_rotation.xmElement.w = w;
}

void AnimChannel::AddScale(const double& dblTimeIn, const float& x, const float& y, const float& z)
{
	vScale.emplace_back();
	AnimScale& current_scale = vScale[vScale.size() - 1];
	current_scale.dblTime = dblTimeIn;
	current_scale.xmElement.x = x;
	current_scale.xmElement.y = y;
	current_scale.xmElement.z = z;
}

XMFLOAT3 AnimChannel::GetTranslation(const double& dblPlayTimeIn)
{
	return GetInterpolatedData(dblPlayTimeIn, vTranslation, XMFLOAT3(0.f, 0.f, 0.f));
}

XMMATRIX AnimChannel::GetTransformation(const double& dblPlayTimeIn)
{
	XMFLOAT3 xmvScaling = GetInterpolatedData(dblPlayTimeIn, vScale, XMFLOAT3(1.f, 1.f, 1.f));
	XMFLOAT4 xmvQuaternion = GetInterpolatedData(dblPlayTimeIn, vRotation, XMFLOAT4(0.f, 0.f, 0.f, 1.f));
	XMFLOAT3 xmvTranslation = GetInterpolatedData(dblPlayTimeIn, vTranslation, XMFLOAT3(0.f, 0.f, 0.f));

	return XMMatrixScaling(xmvScaling.x, xmvScaling.y, xmvScaling.z)
		* XMMatrixRotationQuaternion(XMVectorSet(xmvQuaternion.x, xmvQuaternion.y, xmvQuaternion.z, xmvQuaternion.w))
		* XMMatrixTranslation(xmvTranslation.x, xmvTranslation.y, xmvTranslation.z);
}

DirectX::XMMATRIX AnimChannel::GetTransformationWithoutTranslation(const double& dblPlayTimeIn)
{
	XMFLOAT3 xmvScaling = GetInterpolatedData(dblPlayTimeIn, vScale, XMFLOAT3(1.f, 1.f, 1.f));
	XMFLOAT4 xmvQuaternion = GetInterpolatedData(dblPlayTimeIn, vRotation, XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	return XMMatrixScaling(xmvScaling.x, xmvScaling.y, xmvScaling.z)
		* XMMatrixRotationQuaternion(XMVectorSet(xmvQuaternion.x, xmvQuaternion.y, xmvQuaternion.z, xmvQuaternion.w))
		* XMMatrixTranslation(0.f, 0.f, 0.f);
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
