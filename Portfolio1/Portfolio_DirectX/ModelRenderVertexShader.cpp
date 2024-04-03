#include "ModelRenderVertexShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "PBRStaticMesh.h"
#include "AIBLMesh.h"

#include "ATransformerable.h"
#include "Viewable.h"

using namespace std;
using namespace DirectX;

ModelRenderVertexShader* ModelRenderVertexShader::pPixelShader = nullptr;

ID3D11Buffer* const ModelRenderVertexShader::pNullBuffers[4] = { nullptr, nullptr, nullptr, nullptr };
const UINT ModelRenderVertexShader::pNulls[4] = { NULL, NULL, NULL, NULL };
const UINT ModelRenderVertexShader::uiStrides[4] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2),sizeof(XMFLOAT3),sizeof(XMFLOAT3) };
const UINT ModelRenderVertexShader::uiOffsets[4] = { 0, 0, 0, 0 };

ModelRenderVertexShader::ModelRenderVertexShader()
	: IVertexShader()
{
	Console::Print("Model Render Vertex Shader Generated.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(
		DirectXDevice::pDevice,
		L"PBRModelVS.hlsl", vInputElemDesc,
		cpVertexShader.GetAddressOf(),
		cpInputLayout.GetAddressOf()
	);
}

ModelRenderVertexShader::~ModelRenderVertexShader()
{
}

void ModelRenderVertexShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(cpVertexShader.Get(), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(cpInputLayout.Get());
}

void ModelRenderVertexShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);
}

void ModelRenderVertexShader::SetShader(class ATransformerable& transformable, class Viewable& viewable)
{
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, transformable.GetTransformationBuffer());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, viewable.GetViewProjBuffer());
}

void ModelRenderVertexShader::ResetShader()
{
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
}

void ModelRenderVertexShader::SetIAStage(const size_t& meshIdx, IMesh& mesh)
{
	if (mesh.GetMeshNums() > meshIdx)
	{
		Mesh& meshData = mesh.GetMeshFile()->GetMeshData(meshIdx);

		DirectXDevice::pDeviceContext->IASetPrimitiveTopology(
			dynamic_cast<AIBLMesh*>(&mesh) == nullptr ?
			D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST :
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);

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

void ModelRenderVertexShader::ResetIAStage()
{
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);

	DirectXDevice::pDeviceContext->IASetIndexBuffer(pNullBuffer, DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, pNullBuffers, pNulls, pNulls);
}
