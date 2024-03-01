#include "AStaticMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "ModelManipulator.h"
#include "FileLoader.h"

#include <DirectXMesh.h>

using namespace std;

AStaticMesh::AStaticMesh() 
	: IModel(), ATransformerable()
{

}

AStaticMesh::~AStaticMesh() {}

std::tuple<std::vector<UINT>, std::vector<UINT>, std::vector<ID3D11Buffer*>> AStaticMesh::GetInputInfo()
{
	std::vector<UINT> uiStrides = { sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT2),sizeof(DirectX::XMFLOAT3),sizeof(DirectX::XMFLOAT3) };
	std::vector<UINT> uiOffsets = { 0, 0, 0, 0 };
	std::vector<ID3D11Buffer*> vertexBuffers = {
		spMeshFile->cpVerticesBuffer.Get(),
		spMeshFile->cpTexcoordsBuffer.Get(),
		spMeshFile->cpNormalsBuffer.Get(),
		spMeshFile->cpTangentsBuffer.Get()
	};
	return std::make_tuple(uiStrides, uiOffsets, vertexBuffers);
}

void AStaticMesh::Draw()
{
	DirectXDevice::pDeviceContext->DrawIndexed((UINT)spMeshFile->vIndices.size(), NULL, NULL);
}

void AStaticMesh::UpdateModel(const float& fDelta)
{
	UpdateTranformationMatrix();
	ID3D11Helper::UpdateBuffer(DirectXDevice::pDeviceContext, sTransformation, D3D11_MAP_WRITE_DISCARD, cpTransformationBuffer.Get());
}

void AStaticMesh::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void AStaticMesh::ScaleUp(const float& fXup, const float& fYUp, const float& fZUp)
{
	sScales.fScaleX += fXup;
	sScales.fScaleY += fYUp;
	sScales.fScaleZ += fZUp;

	UpdateTranformationMatrix();
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sTransformation, 
		D3D11_MAP_WRITE_DISCARD, 
		cpTransformationBuffer.Get()
	);
}