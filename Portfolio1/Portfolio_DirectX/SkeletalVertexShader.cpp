#include "SkeletalVertexShader.h"

#include "SkeletalModel.h"
#include "Viewable.h"

using namespace std;
using namespace DirectX;

SkeletalVertexShader* SkeletalVertexShader::pVertexShader = nullptr;

ID3D11Buffer* const SkeletalVertexShader::pNullBuffers[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
const UINT SkeletalVertexShader::pNulls[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
const UINT SkeletalVertexShader::uiStrides[8] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2),sizeof(XMFLOAT3),sizeof(XMFLOAT3), sizeof(XMFLOAT4), sizeof(XMFLOAT4),sizeof(XMUINT4),sizeof(XMUINT4) };
const UINT SkeletalVertexShader::uiOffsets[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

SkeletalVertexShader::SkeletalVertexShader() 
	: IVertexShader()
{
	Console::Print("Model Render Vertex Shader Generated.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 6, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDINDICES", 1, DXGI_FORMAT_R32G32B32A32_UINT, 7, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11Helper::CreateVSInputLayOut(
		DirectXDevice::pDevice,
		L"PBRSkeletalVS.hlsl", vInputElemDesc,
		cpVertexShader.GetAddressOf(),
		cpInputLayout.GetAddressOf()
	);

}

SkeletalVertexShader::~SkeletalVertexShader()
{
}

void SkeletalVertexShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(cpVertexShader.Get(), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(cpInputLayout.Get());
}

void SkeletalVertexShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);
}

void SkeletalVertexShader::SetShader(void* pBindingSet)
{
	SkeletalVSBindingSet* pBinding = (SkeletalVSBindingSet*)pBindingSet;
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pBinding->pSkeletal->GetTransformationBuffer());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pBinding->pViewable->GetViewProjBuffer());
	DirectXDevice::pDeviceContext->VSSetShaderResources(0, 1, pBinding->pSkeletal->GetBoneTransformationBuffer());

}

void SkeletalVertexShader::ResetShader()
{
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetShaderResources(0, 1, &pNullSRV);
}


void SkeletalVertexShader::SetIAStage(const size_t& meshIdx, IMesh& mesh)
{
	if (mesh.GetMeshNums() > meshIdx)
	{
		Mesh& meshData = mesh.GetMeshFile()->GetMeshData(meshIdx);

		DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

		const std::vector<ID3D11Buffer*> vertexBuffers = {
			meshData.cpVerticesBuffer.Get(),
			meshData.cpTexcoordsBuffer.Get(),
			meshData.cpNormalsBuffer.Get(),
			meshData.cpTangentsBuffer.Get(),
			meshData.cpBlendWeight1Buffer.Get(),
			meshData.cpBlendWeight2Buffer.Get(),
			meshData.cpBlendIndices1Buffer.Get(),
			meshData.cpBlendIndices2Buffer.Get()
		};

		DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 8, vertexBuffers.data(), uiStrides, uiOffsets);
		DirectXDevice::pDeviceContext->IASetIndexBuffer(meshData.cpInicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}

void SkeletalVertexShader::ResetIAStage()
{
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);

	DirectXDevice::pDeviceContext->IASetIndexBuffer(pNullBuffer, DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 8, pNullBuffers, pNulls, pNulls);
}
