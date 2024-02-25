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

	CreateCubeModel(
		spVertices, 
		spTexcoords, 
		spNormals, 
		spTangents, 
		spIndicesData, fRadius, true, usLevel);

	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spVertices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpVerticesBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spTexcoords, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpTexcoordsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spNormals, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpNormalsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spTangents, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpTangentsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spIndicesData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpIndexBuffer.GetAddressOf());
}

CubeMapModel::~CubeMapModel()
{
}