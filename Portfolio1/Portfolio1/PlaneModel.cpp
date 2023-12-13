#include "PlaneModel.h"
#include <directxmath/DirectXMath.h>

using namespace DirectX;

PlaneModel::PlaneModel(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn,
	const float& fCenterX, const float& fCenterY, const float& fCenterZ, 
	const float& fWidthIn, const float& fHeightIn,
	const float& fWrapWidthIn, const float& fWrapHeightIn
) : PBRModel(pDeviceIn, pDeviceContextIn)
{
	PickableModelInterface::MakePlaneVertexIndexSet(this, fCenterX, fCenterY, fCenterZ, fWidthIn, fHeightIn, fWrapWidthIn, fWrapHeightIn);
	sTransformation.sAngles.fPitch = -XM_PIDIV2;
}

PlaneModel::~PlaneModel()
{
}
