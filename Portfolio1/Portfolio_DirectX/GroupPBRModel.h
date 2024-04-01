#pragma once
#include "ModelFile.h"
#include "PBRStaticMesh.h"
#include "ATransformerable.h"

class GroupPBRModel : public IObject, public ATransformerable
{
public:
	GroupPBRModel(const std::shared_ptr<ModelFile>& spModelFile);
	virtual ~GroupPBRModel();

protected:
	std::vector<PBRStaticMesh> vChildrenMeshes;

public:
	inline const std::vector<PBRStaticMesh>& GetChildrenMeshes() { return vChildrenMeshes; }
	inline std::vector<PBRStaticMesh>& GetChildrenMeshesRef() { return vChildrenMeshes; }

private:
	virtual void Draw() override {};

public:
	virtual void UpdateModel(const float& fDelta) override;

public:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override;
	virtual void AcceptRenderingLightMap(class LightRenderer* pLightRnederer) override;

protected:
	static uint32_t uiGroupPBRModelIdx;
};

/*
TODO
GroupPBR이 없어져도됨.

SkeletalModel의 생성자에서 BoneFile과 MeshFile을 생성자로 두고,
이를 통해 MeshFile의 input 부분 생성해주기

*/