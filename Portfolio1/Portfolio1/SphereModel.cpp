#include "SphereModel.h"
#include "ID3D11Helper.h"
#include "TransformProperties.h"

using namespace std;
using namespace DirectX;

SphereModel::SphereModel(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const unsigned short& usLevel
) : ObjectModel(cpDeviceIn, cpDeviceContextIn)
{
	ModelInterface::MakeSphereVertexIndexSet(this, fCenterX, fCenterY, fCenterZ, fRadius, usLevel);
}

SphereModel::~SphereModel()
{
}
