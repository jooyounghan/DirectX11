#pragma once
#include <unordered_map>

#include "IMesh.h"
#include "ModelFile.h"
#include "ImageFile.h"

class PBRStaticMesh : public IMesh
{
protected:
	static std::string								strDefaultTextureName;
	static std::unordered_map<WORD, std::string>	unmapTextureNames;

public:
	static const std::string& GetTextureName(const WORD& iTextureID);

public:
	PBRStaticMesh();
	PBRStaticMesh(const MeshFileSet& meshFileSet);
	virtual ~PBRStaticMesh();

private:
	void InitPBRStaticMesh();

protected:
	MeshFileSet		sMeshFileSet;

public:
	inline const bool& IsMeshFileInitailized() { return sMeshFileSet.bIsInitialized; }
	inline std::shared_ptr<MeshFile>& GetMeshFileRef() { return sMeshFileSet.spMeshFile; }
	inline std::shared_ptr<IImageFile>& GetTextureImageFileRef(const EModelTextures& eModelTexture) { return sMeshFileSet.spModelTexture[eModelTexture]; }

protected:
	struct
	{
		float fFresnelConstant[3];
		float fHeightFactor;
	} sPBRConstant;
	struct
	{
		BOOL bIsTextureOn[TEXTURE_MAP_NUM];
		BOOL bIsGLTF;
	} sPBRTextureFlag;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPBRConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPBRTextureFlagBuffer;

public:
	inline float* GetFresnelConstantAddress() { return sPBRConstant.fFresnelConstant; }
	inline float* GetHeightFactorAddress() { return &sPBRConstant.fHeightFactor; }

public:
	inline void SetHeightFactor(const float& fFactor) { sPBRConstant.fHeightFactor = fFactor; }

public:
	inline ID3D11Buffer* const* GetPBRConstantBuffer() { return cpPBRConstantBuffer.GetAddressOf(); }
	inline ID3D11Buffer* const* GetPBRTextureFlagBuffer() { return cpPBRTextureFlagBuffer.GetAddressOf(); }

public:
	virtual void Draw() override;
	virtual void UpdateModel(const float& fDelta) override;

protected:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) override {};
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override {};
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override {};
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override {};
	virtual void AcceptLightMapUpdateSetting(class LightRenderer* pLightRnederer) override {};
};
