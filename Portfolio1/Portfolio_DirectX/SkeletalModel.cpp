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
			if (boneData.strBoneName == "RootNode" && boneData.vBoneChildren.size() == 1)
			{
				UpdateTransformation(boneIdx, boneIdx, boneData.vBoneChildren[0]);
			}
			else
			{
				UpdateTransformation(boneIdx, boneIdx, boneData);
			}

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

	const XMMATRIX parentMatrix = bIsRoot ?
		xmmRootTransform :
		sbBoneTransformation[parentIdx];

	if (pAnimChannel != nullptr)
	{
		if (bIsRoot)
		{
			DirectX::XMVECTOR xmvCurrentTranslation = pAnimChannel->GetTranslation(dblAnimPlayTime);

			if (bIsFirstFrame)
			{
				XMVECTOR xmvRootScaleTranslation;
				XMVECTOR xmvRootRotation;
				XMVECTOR xmvRootTranslation;
				XMMatrixDecompose(
					&xmvRootScaleTranslation, 
					&xmvRootRotation,
					&xmvRootTranslation, 
					xmmRootTransform
				);

				xmmRootTransform = XMMatrixTranslationFromVector(XMVectorSet(
					xmvRootTranslation.m128_f32[0], 
					xmvCurrentTranslation.m128_f32[1],
					xmvRootTranslation.m128_f32[2],
					xmvRootTranslation.m128_f32[3])
				);
			}
			else
			{
				xmmRootTransform = XMMatrixTranslationFromVector(xmvCurrentTranslation - xmvPreviousTranslation) * xmmRootTransform;
			}
			xmmRootTransform = XMMatrixTranslationFromVector(xmvCurrentTranslation - xmvPreviousTranslation) * xmmRootTransform;
			xmvPreviousTranslation = xmvCurrentTranslation;
		}

		sbBoneTransformation[currentIdx] = bIsRoot ?
			pAnimChannel->GetTransformationWithoutTranslation(dblAnimPlayTime) * parentMatrix :
			pAnimChannel->GetTransformation(dblAnimPlayTime) * parentMatrix;

		parentIdx = currentIdx;
		for (BoneData& childBoneData : boneData.vBoneChildren)
		{
			UpdateTransformation(parentIdx, ++currentIdx, childBoneData);
		}

		if (bIsFirstFrame)
		{
			bIsFirstFrame = false;
		}
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
