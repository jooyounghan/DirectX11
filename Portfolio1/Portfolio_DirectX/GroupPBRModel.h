#pragma once
#include "ModelFile.h"
#include "PBRStaticMesh.h"
#include "ATransformerable.h"

class GroupPBRModel : public IMesh, public ATransformerable
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
	virtual void AcceptLightMapUpdateSetting(class LightRenderer* pLightRnederer) override;

protected:
	static uint32_t uiGroupPBRModelIdx;
};

