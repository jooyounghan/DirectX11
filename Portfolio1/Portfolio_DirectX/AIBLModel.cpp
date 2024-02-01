#include "AIBLModel.h"
#include "ModelManipulator.h"

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
