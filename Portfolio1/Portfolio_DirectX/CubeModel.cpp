#include "CubeModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "FileLoader.h"

//TODO : Áö¿ö
#include <string>
using namespace std;

CubeModel::CubeModel(
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const bool& bReverse
)
	: APBRStaticMesh()
{
	xmvPosition.m128_f32[0] = fCenterX;
	xmvPosition.m128_f32[1] = fCenterY;
	xmvPosition.m128_f32[2] = fCenterZ;
	ScaleUp(fRadius - 1.f, fRadius - 1.f, fRadius - 1.f);

	spMeshFile = FileLoader::LoadDefaultCubeMesh(bReverse);
	SetMeshName("Cube Model " + to_string(sModelData.uiModelID));
}

CubeModel::~CubeModel()
{
}