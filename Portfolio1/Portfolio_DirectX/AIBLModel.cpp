#include "AIBLModel.h"
#include "ModelManipulator.h"
#include "ModelRenderer.h"

AIBLModel::AIBLModel()
	: AStaticMesh()
{
}

AIBLModel::~AIBLModel()
{
}

void AIBLModel::AcceptModelManipulator(ModelManipulator* pModelManipulator)
{
	pModelManipulator->VisitModel(*this);
}

void AIBLModel::AcceptModelRenderer(ModelRenderer* pModelRenderer)
{
	pModelRenderer->VisitModel(*this);
}
