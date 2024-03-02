#include <iostream>

#include "MeshFile.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "FileLoader.h"

using namespace std;

MeshFile::MeshFile(const std::string& strFilePathIn, const std::string& strFileNameIn)
	: IFile(strFilePathIn, strFileNameIn)
{

}

MeshFile::~MeshFile()
{
}

void MeshFile::Normalize()
{
    DirectX::XMFLOAT3 vmin(1000, 1000, 1000);
    DirectX::XMFLOAT3 vmax(-1000, -1000, -1000);

    for (auto& v : vVertices) 
    {
        vmin.x = DirectX::XMMin(vmin.x, v.x);
        vmin.y = DirectX::XMMin(vmin.y, v.y);
        vmin.z = DirectX::XMMin(vmin.z, v.z);
        vmax.x = DirectX::XMMax(vmax.x, v.x);
        vmax.y = DirectX::XMMax(vmax.y, v.y);
        vmax.z = DirectX::XMMax(vmax.z, v.z);
    }

    float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
    float scale = 1.f / DirectX::XMMax(DirectX::XMMax(dx, dy), dz);

    DirectX::XMFLOAT3 translation;
    translation.x = -(vmin.x + vmax.x) * 0.5f;
    translation.y = -(vmin.y + vmax.y) * 0.5f;
    translation.z = -(vmin.z + vmax.z) * 0.5f;

    for (auto& v : vVertices) {
        v.x = (v.x + translation.x) * scale;
        v.y = (v.y + translation.y) * scale;
        v.z = (v.z + translation.z) * scale;
    }
}

void MeshFile::CreateBuffers()
{
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vVertices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpVerticesBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vTexcoords, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpTexcoordsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vNormals, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpNormalsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vTangents, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpTangentsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vIndices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpInicesBuffer.GetAddressOf());
}

void MeshFile::UpdateTangents()
{
	HRESULT hResult = DirectX::ComputeTangentFrame(
		vIndices.data(),
		vIndices.size() / 3,
		vVertices.data(),
		vNormals.data(),
		vTexcoords.data(),
		vVertices.size(),
		vTangents.data(),
		nullptr
	);
	if (FAILED(hResult)) { cout << "Computing Tangent Frame Failed" << endl; }
}
