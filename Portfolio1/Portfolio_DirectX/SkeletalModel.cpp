#include "SkeletalModel.h"
#include "ModelManipulator.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "DefineVar.h"

using namespace std;
using namespace DirectX;

SkeletalModel::SkeletalModel(
	const std::shared_ptr<MeshFile>& spMeshFileIn
)
	: PBRStaticMesh(spMeshFileIn),
	dblAnimPlayTime(0.), xmmRootTransform(XMMatrixIdentity()),
	xmvPreviousTranslation(XMVectorSet(0.f, 0.f, 0.f, 0.f)),
	IMovable(0.f, 0.f, 0.f),
	IScalable(),
	IAngleAdjustable(0.f, 0.f, 0.f)
{
	AutoZeroMemory(sAnimData);
	sAnimData.dblAnimSpeed = 1.0;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sAnimData, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpAnimationBuffer.GetAddressOf()
	);

	BoneFile* pBoneFile = spMeshFile->GetBoneFile();
	if (pBoneFile != nullptr)
	{
		vBoneTransformation.resize(pBoneFile->GetBoneNums(), XMMatrixIdentity());
	}
}

SkeletalModel::~SkeletalModel()
{
}

void SkeletalModel::AddPlayTime(const float& fDeltaTime)
{
	if (spAnimFile.get() != nullptr)
	{
		const double& dblDuration = spAnimFile->GetDuration();
		const double& dblTicksPerSecond = spAnimFile->GetTicksPerSecond();

		dblAnimPlayTime += static_cast<double>(fDeltaTime) * dblTicksPerSecond * sAnimData.dblAnimSpeed;
		while (dblAnimPlayTime > dblDuration)
		{
			dblAnimPlayTime -= dblDuration;
		}
	}
}

void SkeletalModel::UpdateModel(const float& fDelta)
{
	AddPlayTime(fDelta);
	PBRStaticMesh::UpdateModel(fDelta);

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sAnimData, D3D11_MAP_WRITE_DISCARD,
		cpAnimationBuffer.Get()
	);

	//if (spAnimFile.get() != nullptr && spBoneFile.get() != nullptr)
	//{
	//	std::unordered_map<std::string, AnimChannelFile>& unmapAnimChannels = spAnimFile->GetAnimationChannels();
	//	for (auto& animChannel : unmapAnimChannels)
	//	{
	//		const string& strChannelName = animChannel.second.GetAnimChannelName();
	//		if (skeletalTransformation.find(strChannelName) != skeletalTransformation.end())
	//		{
	//			skeletalTransformation[strChannelName] = animChannel.second.GetChannelTransformation(dblAnimPlayTime);
	//		}
	//	}

	//	UpdateBoneTransformation(*pBone);
	//}
}

//void SkeletalModel::UpdateBoneTransformation(Bone& bone)
//{
//	const string& strParentBoneName = bone.GetBoneName();
//
//	if (skeletalTransformation.find(strParentBoneName) != skeletalTransformation.end())
//	{
//		bool isRootBone = bone.IsRootBone();
//		const XMMATRIX& parentMatrix = isRootBone ? xmmRootTransform : skeletalTransformation[strParentBoneName];
//
//		for (auto& child : bone.GetBoneChildren())
//		{
//			const string& strChildBoneName = child.GetBoneName();
//			if (skeletalTransformation.find(strChildBoneName) != skeletalTransformation.end())
//			{
//				if (isRootBone)
//				{
//					XMVECTOR xmvDummy;
//					XMVECTOR xmvTranslation;
//					XMMatrixDecompose(&xmvDummy, &xmvDummy, &xmvTranslation, skeletalTransformation[strChildBoneName]);					
//					xmmRootTransform = XMMatrixTranslationFromVector(xmvTranslation - xmvPreviousTranslation) * xmmRootTransform;
//					xmvPreviousTranslation = xmvTranslation;
//				}
//
//				skeletalTransformation[strChildBoneName] = skeletalTransformation[strChildBoneName] * parentMatrix;
//				UpdateBoneTransformation(child);
//			}
//		}
//	}
//}

void SkeletalModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}
