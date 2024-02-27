#include "CubeModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
//TODO : Áö¿ö
#include <string>
using namespace std;

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

	CreateCubeModel(
		spVertices,
		spTexcoords,
		spNormals,
		spTangents,
		spIndicesData, fRadius, bReverse, usLevel);

	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spVertices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpVerticesBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spTexcoords, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpTexcoordsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spNormals, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpNormalsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spTangents, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpTangentsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, spIndicesData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpIndexBuffer.GetAddressOf());

	SetMeshName("Cube Model" + to_string(sModelData.uiModelID));
}

CubeModel::~CubeModel()
{
}