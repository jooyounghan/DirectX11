#pragma once
#include "PBRStaticMesh.h"
#include "BoneFile.h"
#include "AnimationFile.h"
#include "StructuredBuffer.h"

class SkeletalModel : public PBRStaticMesh
{
public:
	SkeletalModel(
		const std::shared_ptr<MeshFile>& spMeshFileIn
	);
	virtual ~SkeletalModel();

protected:
	std::shared_ptr<AnimationFile>	spAnimFile;

protected:
	DirectX::XMMATRIX xmmRootTransform;
	DirectX::XMFLOAT3 xmvPreviousTranslation;
	bool bIsFirstFrame;

public:
	inline void SetAnimationFile(const std::shared_ptr<AnimationFile>& spAnimFileIn) { spAnimFile = spAnimFileIn; }
	inline AnimationFile* GetAnimationFile() { return spAnimFile.get(); }

protected:
	double dblAnimPlayTime;
	struct
	{
		double dblAnimSpeed;
		double dblDummy;
	} sAnimData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpAnimationBuffer;

public:
	inline ID3D11Buffer* const* GetAnimationBuffer() { return cpAnimationBuffer.GetAddressOf(); }

protected:
	StructuredBuffer<DirectX::XMMATRIX> sbBoneTransformation;

public:
	inline ID3D11ShaderResourceView* const* GetBoneTransformationBuffer() { return sbBoneTransformation.GetStructuredSRV(); }

public:
	void SetAnimSpeed(const double& dblSpeed) { sAnimData.dblAnimSpeed = dblSpeed; }
	const double& GetAnimSpeed() { return sAnimData.dblAnimSpeed; }
	double* GetAnimSpeedAddress() { return &sAnimData.dblAnimSpeed; }

public:
	void AddPlayTime(const float& fDeltaTime);
	inline const double& GetPlayTime() { return dblAnimPlayTime; }

public:
	virtual void UpdateModel(const float& fDelta) override;
	void UpdateTransformation(size_t parentIdx, size_t& boneIdx, BoneData& boneData);

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;
	virtual void AcceptRenderingLightMap(class LightRenderer* pLightRnederer) override;
};

