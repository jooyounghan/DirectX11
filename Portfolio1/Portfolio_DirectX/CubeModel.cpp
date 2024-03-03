#include "CubeModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "FileLoader.h"

using namespace std;

uint32_t CubeModel::uiCubeModelIdx = 1;

CubeModel::CubeModel(
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const bool& bReverse
)
	: SinglePBRModel()
{
	sMeshFileSet.spMeshFile = FileLoader::LoadDefaultCubeMesh(bReverse);
	SetPosition(fCenterX, fCenterY, fCenterZ);
	ScaleUp(fRadius - 1.f, fRadius - 1.f, fRadius - 1.f);
	UpdateTranformationMatrix();

	SetMeshName("Cube Model " + to_string(uiCubeModelIdx));
	uiCubeModelIdx++;
}

CubeModel::~CubeModel()
{
}