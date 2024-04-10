#include "NormalVectorVertexShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ATransformerable.h"
#include "PBRStaticMesh.h"

using namespace std;

NormalVectorVertexShader* NormalVectorVertexShader::pVertexShader = nullptr;

ID3D11Buffer* const NormalVectorVertexShader::pNullBuffers[4] = { nullptr, nullptr, nullptr, nullptr };
UINT NormalVectorVertexShader::pNulls[4] = { NULL, NULL, NULL, NULL };
const UINT NormalVectorVertexShader::uiStrides[4] = { sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT2),sizeof(DirectX::XMFLOAT3),sizeof(DirectX::XMFLOAT3) };
const UINT NormalVectorVertexShader::uiOffsets[4] = { 0, 0, 0, 0 };

NormalVectorVertexShader::NormalVectorVertexShader()
	: IVertexShader()
{
	Console::Print("Normal Vector Vertex Shader Generated.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(
		DirectXDevice::pDevice, 
		L"NormalVectorVS.hlsl", vInputElemDesc,
		cpVertexShader.GetAddressOf(),
		cpInputLayout.GetAddressOf()
	);
}

NormalVectorVertexShader::~NormalVectorVertexShader()
{
}

void NormalVectorVertexShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(cpVertexShader.Get(), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(cpInputLayout.Get());
}

void NormalVectorVertexShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);
}

void NormalVectorVertexShader::SetShader(class ATransformerable& transformable)
{
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, transformable.GetTransformationBuffer());
}

void NormalVectorVertexShader::ResetShader()
{
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffers[0]);
}

void NormalVectorVertexShader::SetIAStage(const size_t& meshIdx, IMesh& mesh)
{
	if (mesh.GetMeshNums() > meshIdx)
	{
		Mesh& meshData = mesh.GetMeshFile()->GetMeshData(meshIdx);

		DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		const std::vector<ID3D11Buffer*> vertexBuffers = {
			meshData.cpVerticesBuffer.Get(),
			meshData.cpTexcoordsBuffer.Get(),
			meshData.cpNormalsBuffer.Get(),
			meshData.cpTangentsBuffer.Get()
		};

		DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, vertexBuffers.data(), uiStrides, uiOffsets);
		DirectXDevice::pDeviceContext->IASetIndexBuffer(meshData.cpInicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}

void NormalVectorVertexShader::ResetIAStage()
{
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);

	DirectXDevice::pDeviceContext->IASetIndexBuffer(pNullBuffers[0], DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, pNullBuffers, pNulls, pNulls);
}
