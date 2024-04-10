#pragma once
#include <unordered_map>

#include "IMesh.h"
#include "MaterialFile.h"
#include "ATransformerable.h"

class PBRStaticMesh : public IMesh, public ATransformerable
{
public:
	PBRStaticMesh(const std::shared_ptr<MeshFile>& spMeshFileIn);
	virtual ~PBRStaticMesh();

protected:
	static uint32_t uiPBRStaticMeshlIdx;

protected:
	struct
	{
		float fFresnelConstant[3];
		float fHeightFactor;
	} sPBRConstant;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPBRConstantBuffer;

public:
	inline float* GetFresnelConstantAddress() { return sPBRConstant.fFresnelConstant; }
	inline float* GetHeightFactorAddress() { return &sPBRConstant.fHeightFactor; }

public:
	inline void SetHeightFactor(const float& fFactor) { sPBRConstant.fHeightFactor = fFactor; }

public:
	inline ID3D11Buffer* const* GetPBRConstantBuffer() { return cpPBRConstantBuffer.GetAddressOf(); }

public:
	virtual void UpdateModel(const float& fDelta) override;

protected:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override;
	virtual void AcceptRenderingLightMap(class LightRenderer* pLightRnederer) override;
};
