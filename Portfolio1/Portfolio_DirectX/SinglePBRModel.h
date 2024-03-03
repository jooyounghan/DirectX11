#pragma once
#include "PBRStaticMesh.h"
#include "ATransformerable.h"

class SinglePBRModel : public PBRStaticMesh, public ATransformerable
{
public:
	SinglePBRModel();
	virtual ~SinglePBRModel();

public:
	virtual void UpdateModel(const float& fDelta) override;

public:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override;
	virtual void AcceptLightMapUpdateSetting(class LightRenderer* pLightRnederer) override;
};

