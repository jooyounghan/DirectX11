#include "SinglePBRModel.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "NormalVectorRenderer.h"
#include "LightRenderer.h"

SinglePBRModel::SinglePBRModel()
	: PBRStaticMesh(), ATransformerable(),
	IMovable(0.f, 0.f, 0.f),
	IScalable(),
	IAngleAdjustable(0.f, 0.f, 0.f)
{

}

SinglePBRModel::~SinglePBRModel()
{
}

void SinglePBRModel::UpdateModel(const float& fDelta)
{
	UpdateTranformationMatrix();
	PBRStaticMesh::UpdateModel(fDelta);
}

void SinglePBRModel::AcceptModelAsList(ModelManipulator* pModelManipulator)
{
	pModelManipulator->SetModelAsList(*this);
}

void SinglePBRModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void SinglePBRModel::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}

void SinglePBRModel::AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer)
{
	pNormalVectorRenderer->RenderNormal(*this);
}

void SinglePBRModel::AcceptRenderingLightMap(LightRenderer* pLightRenderer)
{
	pLightRenderer->RenderLightMap(*this);
}
