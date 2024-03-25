#pragma once
#include <memory>

#include "IObject.h"

class IMesh : public IObject
{
public:
	IMesh(std::shared_ptr<MeshFile> spMeshFileIn);
	~IMesh();

protected:
	std::shared_ptr<MeshFile>		spMeshFile;

public:
	inline std::shared_ptr<MeshFile>& GetMeshFileRef() { return spMeshFile; };

public:
	virtual void Draw() = 0;
	virtual void UpdateModel(const float& fDelta) = 0;

public:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) = 0;
	virtual void AcceptModelManipulating(ModelManipulator* pModelManipulator) = 0;
	virtual void AcceptModelRendering(ModelRenderer* pModelRenderer) = 0;
	virtual void AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer) = 0;
	virtual void AcceptRenderingLightMap(LightRenderer* pLightRnederer) = 0;
};
