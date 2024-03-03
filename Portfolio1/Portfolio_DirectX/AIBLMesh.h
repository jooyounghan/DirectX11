#pragma once
#include "IMesh.h"
#include "ATransformerable.h"
#include "MeshFile.h"
#include "ImageFile.h"
#include <memory>

class AIBLMesh : public IMesh, public ATransformerable
{
public:
	AIBLMesh();
	virtual ~AIBLMesh();

protected:
	std::shared_ptr<MeshFile> spMeshFile;

protected:
	struct
	{
		float DiffuseRate;
		float fDummy[3];
	} sIBLData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpIBLDataBuffer;

public:
	inline float* GetDiffuseRateAddress() { return &sIBLData.DiffuseRate; }
	inline ID3D11Buffer* const* GetIBLDataBuffer() { return cpIBLDataBuffer.GetAddressOf(); }

protected:
	std::shared_ptr<class IImageFile>		spEnvSpecularTextureFile;
	std::shared_ptr<class IImageFile>		spEnvDiffuseTextureFile;
	std::shared_ptr<class IImageFile>		spEnvBrdfTextureFile;

public:
	inline std::shared_ptr<IImageFile>& GetSpecularTextureFileRef() { return spEnvSpecularTextureFile; }
	inline std::shared_ptr<IImageFile>& GetDiffuseTextureFileRef() { return spEnvDiffuseTextureFile; }
	inline std::shared_ptr<IImageFile>& GetBRDFTextureFileRef() { return spEnvBrdfTextureFile; }

public:
	virtual void Draw() override;
	virtual void UpdateModel(const float& fDelta) override;

private:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;

private:
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override;
	virtual void AcceptLightMapUpdateSetting(class LightRenderer* pLightRnederer) override;
};

