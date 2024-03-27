#include "PBRStaticMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "LightRenderer.h"
#include "NormalVectorRenderer.h"

using namespace std;

PBRStaticMesh::PBRStaticMesh(const std::shared_ptr<MeshFile>& spMeshFileIn)
	: IMesh(spMeshFileIn)
{
	AutoZeroMemory(sPBRConstant);

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sPBRConstant, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpPBRConstantBuffer.GetAddressOf()
	);
}

PBRStaticMesh::~PBRStaticMesh()
{
}

void PBRStaticMesh::Draw()
{
	DirectXDevice::pDeviceContext->DrawIndexed(spMeshFile->GetIndicesCount(), NULL, NULL);
}

void PBRStaticMesh::UpdateModel(const float& fDelta)
{
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sPBRConstant, D3D11_MAP_WRITE_DISCARD,
		cpPBRConstantBuffer.Get()
	);
}
