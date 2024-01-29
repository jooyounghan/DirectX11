#include "ModelManipulator.h"
#include "AStaticMesh.h"

using namespace ImGui;

ModelManipulator::ModelManipulator(AStaticMesh** ppSelectedStaticMeshIn)
	: ppSelectedStaticMesh(ppSelectedStaticMeshIn)
{
}

ModelManipulator::~ModelManipulator()
{
}

void ModelManipulator::AttachToBar()
{
	AStaticMesh* pSelectedModel = *ppSelectedStaticMesh;
	if (CollapsingHeader("Transformation"))
	{
		DragFloat3("Translation", pSelectedModel->xmvPosition.m128_f32, 1.f, -1000000.f, 1000000.f, "%.2f");
		DragFloat3("Rotaion", &pSelectedModel->sAnglesDegree.fPitchDeg, 1.f, 0.f, 360.f, "%.2f");
		DragFloat3("Scale", &pSelectedModel->sScales.fScaleX, 0.005f, -1000000.f, 1000000.f, "%.2f");
	}
}

