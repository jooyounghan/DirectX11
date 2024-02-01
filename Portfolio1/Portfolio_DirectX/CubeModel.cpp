#include "CubeModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

CubeModel::CubeModel(
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const bool& bReverse,
	const uint32_t& usLevel
)
	: APBRStaticMesh()
{
	xmvPosition.m128_f32[0] = fCenterX;
	xmvPosition.m128_f32[1] = fCenterY;
	xmvPosition.m128_f32[2] = fCenterZ;

	CreateCubeModel(inputData, indexData, fRadius, bReverse, usLevel);

	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, inputData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpInputBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, indexData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpIndexBuffer.GetAddressOf());
}

CubeModel::~CubeModel()
{
}


void CubeModel::UpdateModel(const float& fDelta)
{
	APBRStaticMesh::UpdateModel(fDelta);
}
