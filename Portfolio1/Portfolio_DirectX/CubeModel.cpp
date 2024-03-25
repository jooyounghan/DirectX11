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
	: GroupPBRModel(FileLoader::LoadDefaultCubeModel(bReverse)),
	IMovable(fCenterX, fCenterY, fCenterZ),
	IScalable(),
	IAngleAdjustable(0.f, 0.f, 0.f)
{
	ScaleUp(fRadius - 1.f, fRadius - 1.f, fRadius - 1.f);
	UpdateTranformationMatrix();

	SetObjectName("Cube Model " + to_string(uiCubeModelIdx));
	uiCubeModelIdx++;
}

CubeModel::~CubeModel()
{
}