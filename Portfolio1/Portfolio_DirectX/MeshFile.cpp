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
