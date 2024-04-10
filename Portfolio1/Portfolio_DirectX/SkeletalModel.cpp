#include "SkeletalModel.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"

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
	sbBoneTransformation(spMeshFile->GetBoneFile()->GetBoneNums(), XMMatrixIdentity()),
	bIsFirstFrame(true),
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
			bIsFirstFrame = true;
			dblAnimPlayTime -= dblDuration;
		}
	}
}

void SkeletalModel::UpdateModel(const float& fDelta)
{
	PBRStaticMesh::UpdateModel(fDelta);
	
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sAnimData, D3D11_MAP_WRITE_DISCARD,
		cpAnimationBuffer.Get()
	);

	AddPlayTime(fDelta);
	MeshFile* const pMeshFile = GetMeshFile();
	if (pMeshFile != nullptr)
	{
		const XMMATRIX& normalizedMatrix = pMeshFile->GetNormalizedMatrix();
		BoneFile* const pBoneFile = pMeshFile->GetBoneFile();
		if (pBoneFile != nullptr && spAnimFile.get())
		{
			size_t boneIdx = 0;
			BoneData& boneData = pBoneFile->GetRootBone();
			boneData.vBoneChildren.size() == 1 ?
				UpdateTransformation(boneIdx, boneIdx, boneData.vBoneChildren[0])
				: UpdateTransformation(boneIdx, boneIdx, boneData);

			for (size_t idx = 0; idx < pBoneFile->GetBoneNums(); ++idx)
			{
				sbBoneTransformation[idx] = XMMatrixTranspose(
					XMMatrixInverse(nullptr, normalizedMatrix)
					* pBoneFile->GetOffsetMatrix(idx) 
					* sbBoneTransformation[idx]
					* normalizedMatrix
				);
			}

			sbBoneTransformation.UpdateStructuredBuffer();

		}
	}
}

void SkeletalModel::UpdateTransformation(size_t parentIdx, size_t& currentIdx, BoneData& boneData)
{
	bool bIsRoot = parentIdx == currentIdx;

	AnimChannel* pAnimChannel = spAnimFile->GetAnimChannel(boneData.strBoneName);

	if (bIsRoot && pAnimChannel != nullptr)
	{
		DirectX::XMVECTOR xmvCurrentTranslation = pAnimChannel->GetTranslation(dblAnimPlayTime);

		if (bIsFirstFrame)
		{
			xmmRootTransform = XMMatrixTranslationFromVector(XMVectorSet(0.f, xmvCurrentTranslation.m128_f32[1], 0.f, 0.f));
		}
		else
		{
			xmmRootTransform = XMMatrixTranslationFromVector(xmvCurrentTranslation - xmvPreviousTranslation) * xmmRootTransform;
		}
		xmvPreviousTranslation = xmvCurrentTranslation;

	}

	const XMMATRIX& parentMatrix = bIsRoot ?
		xmmRootTransform :
		sbBoneTransformation[parentIdx];

	sbBoneTransformation[currentIdx] = pAnimChannel != nullptr ?
		pAnimChannel->GetTransformation(dblAnimPlayTime) * parentMatrix :
		XMMatrixIdentity() * parentMatrix;

	parentIdx = currentIdx;
	for (BoneData& childBoneData : boneData.vBoneChildren)
	{
		UpdateTransformation(parentIdx, ++currentIdx, childBoneData);
	}
}

void SkeletalModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void SkeletalModel::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}
