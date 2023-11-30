#include "SquareModel.h"
#include "ID3D11Helper.h"
#include "TransformProperties.h"

using namespace std;
using namespace DirectX;

SquareModel::SquareModel(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fLen
) : ObjectModel(cpDeviceIn, cpDeviceContextIn)
{
	PickableModel::MakeSquareVertexIndexSet(this, fCenterX, fCenterY, fCenterZ, fLen);
}

SquareModel::~SquareModel()
{
}
