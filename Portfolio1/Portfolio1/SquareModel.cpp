#include "SquareModel.h"
#include "ID3D11Helper.h"

using namespace std;
using namespace DirectX;

SquareModel::SquareModel(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fLen
) : PBRModel(pDeviceIn, pDeviceContextIn)
{
	PickableModelInterface::MakeSquareVertexIndexSet(this, fCenterX, fCenterY, fCenterZ, fLen);
}

SquareModel::~SquareModel()
{
}
