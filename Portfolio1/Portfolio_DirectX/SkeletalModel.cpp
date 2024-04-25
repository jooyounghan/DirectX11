#include "SkeletalModel.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "LightRenderer.h"

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
	xmvPreviousTranslation(XMFLOAT3(0.f, 0.f, 0.f)),
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

			UpdateTransformation(boneIdx, boneIdx, boneData);

			for (size_t idx = 0; idx < pBoneFile->GetBoneNums(); ++idx)
			{
				// TODO : 해당 부분 최적화
				
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
			XMFLOAT3 xmvCurrentTranslation = pAnimChannel->GetTranslation(dblAnimPlayTime);

			if (bIsFirstFrame)
			{
				xmmRootTransform.r[3].m128_f32[1] = xmvCurrentTranslation.y;
			}
			else
			{
				xmmRootTransform = XMMatrixTranslation(
					xmvCurrentTranslation.x - xmvPreviousTranslation.x,
					xmvCurrentTranslation.y - xmvPreviousTranslation.y,
					xmvCurrentTranslation.z - xmvPreviousTranslation.z					
				) * xmmRootTransform;
			}
			xmvPreviousTranslation = xmvCurrentTranslation;
		}

		sbBoneTransformation[currentIdx] = bIsRoot ?
			pAnimChannel->GetTransformationWithoutTranslation(dblAnimPlayTime) * parentMatrix :
			pAnimChannel->GetTransformation(dblAnimPlayTime) * parentMatrix;

	}
	else
	{
		sbBoneTransformation[currentIdx] = parentMatrix;
	}

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

void SkeletalModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void SkeletalModel::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}

void SkeletalModel::AcceptRenderingLightMap(LightRenderer* pLightRnederer)
{
	pLightRnederer->RenderLightMap(*this);
}
