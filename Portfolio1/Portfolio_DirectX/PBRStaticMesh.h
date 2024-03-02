#pragma once
#include <unordered_map>

#include "AStaticMesh.h"
#include "ModelFile.h"
#include "ImageFile.h"

class PBRStaticMesh : public AStaticMesh
{
	friend class ModelRenderer;
	friend class LightRenderer;
	friend class NormalVectorRenderer;

public:
	static std::unordered_map<WORD, std::string> unmapTextureNames;

public:
	PBRStaticMesh();
	PBRStaticMesh(const struct MeshFileSet& meshFileSetIn);
	virtual ~PBRStaticMesh();

public:
	std::shared_ptr<IImageFile> pModelTexture[TEXTURE_MAP_NUM];

public:
	struct
	{
		float fFresnelConstant[3];
		float fHeightFactor;
	} sPBRConstant;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPBRConstantBuffer;

	struct
	{
		BOOL bIsTextureOn[TEXTURE_MAP_NUM];
		BOOL bIsGLTF;
	} sPBRTextureFlag;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPBRTextureFlagBuffer;

public:
	virtual void UpdateModel(const float& fDelta) override;

private:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;

private:
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override;
	virtual void AcceptLightMapUpdating(class LightRenderer* pLightRnederer) override;
};

