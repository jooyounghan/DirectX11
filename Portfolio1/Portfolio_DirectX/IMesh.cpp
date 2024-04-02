#include "IMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

IMesh::IMesh(std::shared_ptr<MeshFile> spMeshFileIn)
	: spMeshFile(spMeshFileIn)
{
	
}

IMesh::~IMesh() {}

void IMesh::Draw(const size_t& meshDataIdx)
{
	DirectXDevice::pDeviceContext->DrawIndexed(spMeshFile->GetMeshData(meshDataIdx).vIndices.size(), NULL, NULL);
}
