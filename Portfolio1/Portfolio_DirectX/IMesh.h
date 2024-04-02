#pragma once
#include <memory>
#include "MeshFile.h"
#include "IObject.h"

class IMesh : public IObject
{
public:
	IMesh(std::shared_ptr<MeshFile> spMeshFileIn);
	~IMesh();

protected:
	std::shared_ptr<MeshFile>		spMeshFile;

public:
	inline MeshFile* GetMeshFile() { return spMeshFile.get(); };
	inline size_t GetMeshNums() { return spMeshFile.get() != nullptr ? spMeshFile->GetMeshNums() : 0; }

public:
	virtual void Draw(const size_t& meshDataIdx);
	virtual void UpdateModel(const float& fDelta) = 0;

public:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) = 0;
	virtual void AcceptModelManipulating(ModelManipulator* pModelManipulator) = 0;
	virtual void AcceptModelRendering(ModelRenderer* pModelRenderer) = 0;
	virtual void AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer) = 0;
	virtual void AcceptRenderingLightMap(LightRenderer* pLightRnederer) = 0;
};
