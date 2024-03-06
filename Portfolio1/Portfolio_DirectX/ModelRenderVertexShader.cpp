#include "ModelRenderVertexShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "PBRStaticMesh.h"
#include "ATransformerable.h"
#include "Viewable.h"

using namespace std;

ID3D11Buffer* const ModelRenderVertexShader::pNullBuffer[4] = { nullptr, nullptr, nullptr, nullptr };
UINT ModelRenderVertexShader::pNull[4] = { NULL, NULL, NULL, NULL };
const vector<UINT> ModelRenderVertexShader::uiStrides = { sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT2),sizeof(DirectX::XMFLOAT3),sizeof(DirectX::XMFLOAT3) };
const vector<UINT> ModelRenderVertexShader::uiOffsets = { 0, 0, 0, 0 };

ModelRenderVertexShader::ModelRenderVertexShader()
	: IVertexShader()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(
		DirectXDevice::pDevice,
		L"BaseVS.hlsl", vInputElemDesc,
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
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}

void ModelRenderVertexShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
}

void ModelRenderVertexShader::SetShader(class ATransformerable& transformable, class Viewable* pViewable)
{
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, transformable.GetTransformationBuffer());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pViewable->GetViewProjBuffer());
}

void ModelRenderVertexShader::ResetShader()
{
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer[0]);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer[0]);
}

void ModelRenderVertexShader::SetIAStage(PBRStaticMesh& pbrStaticMesh)
{
	const std::vector<ID3D11Buffer*> vertexBuffers = {
		pbrStaticMesh.GetMeshFileRef()->cpVerticesBuffer.Get(),
		pbrStaticMesh.GetMeshFileRef()->cpTexcoordsBuffer.Get(),
		pbrStaticMesh.GetMeshFileRef()->cpNormalsBuffer.Get(),
		pbrStaticMesh.GetMeshFileRef()->cpTangentsBuffer.Get()
	};

	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, vertexBuffers.data(), uiStrides.data(), uiOffsets.data());
	DirectXDevice::pDeviceContext->IASetIndexBuffer(pbrStaticMesh.GetMeshFileRef()->cpInicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void ModelRenderVertexShader::ResetIAStage()
{
	DirectXDevice::pDeviceContext->IASetIndexBuffer(pNullBuffer[0], DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, pNullBuffer, pNull, pNull);
}
