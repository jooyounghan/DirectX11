#pragma once
#include <unordered_map>

#include "IMesh.h"
#include "MaterialFile.h"
#include "ATransformerable.h"

class MaterialFile;

class PBRStaticMesh : public IMesh, public ATransformerable
{
public:
	PBRStaticMesh(const std::shared_ptr<MeshFile>& spMeshFileIn);
	virtual ~PBRStaticMesh();

protected:
	static uint32_t uiPBRStaticMeshlIdx;

protected:
	std::vector<std::shared_ptr<MaterialFile>> vMaterials;

public:
	inline void SetMaterialFile(const size_t& materialIdx, const std::shared_ptr<MaterialFile>& spMaterialFileIn) {
		if (vMaterials.size() > materialIdx)
		{
			vMaterials[materialIdx] = spMaterialFileIn;
		}
	}
	inline MaterialFile* GetMaterialFile(const size_t& materialIdx) { return vMaterials.size() > materialIdx ? vMaterials[materialIdx].get() : nullptr; }

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
