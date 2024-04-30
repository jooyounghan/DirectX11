#include "BasicVertexShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "IMesh.h"
#include "MeshFile.h"
#include "ATransformerable.h"
#include "Viewable.h"

using namespace std;

BasicVertexShader* BasicVertexShader::pVertexShader = nullptr;

ID3D11Buffer* const BasicVertexShader::pNullBuffers[3] = { nullptr, nullptr, nullptr };
const UINT BasicVertexShader::pNulls[3] = { NULL, NULL, NULL };
const UINT BasicVertexShader::uiStrides[3] = { sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT2),sizeof(DirectX::XMFLOAT3) };
const UINT BasicVertexShader::uiOffsets[3] = { 0, 0, 0 };

BasicVertexShader::BasicVertexShader()
	: IVertexShader()
{
	Console::Print("Basic Vertex Shader Generated.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(
		DirectXDevice::pDevice,
		L"BasicVS.hlsl", vInputElemDesc,
		cpVertexShader.GetAddressOf(),
		cpInputLayout.GetAddressOf()
	);
}
BasicVertexShader::~BasicVertexShader()
{
}

void BasicVertexShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(cpVertexShader.Get(), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(cpInputLayout.Get());
}

void BasicVertexShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);
}

void BasicVertexShader::SetShader(void* pBindingSet)
{
	BasicVSBindingSet* pBinding = (BasicVSBindingSet*)pBindingSet;
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pBinding->pTransformable->GetTransformationBuffer());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pBinding->pViewable->GetViewProjBuffer());
}

void BasicVertexShader::ResetShader()
{
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
}

void BasicVertexShader::SetIAStage(const size_t& meshIdx, IMesh& mesh)
{
	if (mesh.GetMeshNums() > meshIdx)
	{
		Mesh& meshData = mesh.GetMeshFile()->GetMeshData(meshIdx);

		DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		const std::vector<ID3D11Buffer*> vertexBuffers = {
			meshData.cpVerticesBuffer.Get(),
			meshData.cpTexcoordsBuffer.Get(),
			meshData.cpNormalsBuffer.Get()
		};

		DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 3, vertexBuffers.data(), uiStrides, uiOffsets);
		DirectXDevice::pDeviceContext->IASetIndexBuffer(meshData.cpInicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}

void BasicVertexShader::ResetIAStage()
{
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);

	DirectXDevice::pDeviceContext->IASetIndexBuffer(pNullBuffer, DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 3, pNullBuffers, pNulls, pNulls);
}
