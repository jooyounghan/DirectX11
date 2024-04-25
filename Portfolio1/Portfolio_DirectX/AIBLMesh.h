#pragma once
#include "IMesh.h"
#include "ATransformerable.h"
#include "MeshFile.h"
#include <memory>

class AIBLMesh : public IMesh, public ATransformerable
{
public:
	AIBLMesh(std::shared_ptr<MeshFile> spMeshFileIn);
	virtual ~AIBLMesh();

protected:
	std::shared_ptr<class IImageFile>		spIBLTextureFile;
	std::shared_ptr<class IImageFile>		spEnvSpecularTextureFile;
	std::shared_ptr<class IImageFile>		spEnvDiffuseTextureFile;
	std::shared_ptr<class IImageFile>		spEnvBrdfTextureFile;

public:
	inline std::shared_ptr<IImageFile>& GetIBLTextureFile() { return spIBLTextureFile; }
	inline std::shared_ptr<IImageFile>& GetSpecularTextureFileRef() { return spEnvSpecularTextureFile; }
	inline std::shared_ptr<IImageFile>& GetDiffuseTextureFileRef() { return spEnvDiffuseTextureFile; }
	inline std::shared_ptr<IImageFile>& GetBRDFTextureFileRef() { return spEnvBrdfTextureFile; }

private:
	static ID3D11Buffer* const pNullBuffers[4];
	static UINT pNulls[4];
	static const std::vector<UINT> uiStrides;
	static const std::vector<UINT> uiOffsets;

public:
	virtual void UpdateModel(const float& fDelta) override;

private:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;

private:
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override;
	virtual void AcceptRenderingLightMap(class LightRenderer* pLightRnederer) override;
};

