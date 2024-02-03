#include "CubeMapModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

CubeMapModel::CubeMapModel(
	const float& fRadius, 
	const uint32_t& usLevel
)
	: AIBLModel()
{
	xmvPosition.m128_f32[0] = 0.f;
	xmvPosition.m128_f32[1] = 0.f;
	xmvPosition.m128_f32[2] = 0.f;

	CreateCubeModel(inputData, indexData, fRadius, true, usLevel);

	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, inputData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpInputBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, indexData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpIndexBuffer.GetAddressOf());
}

CubeMapModel::~CubeMapModel()
{
}