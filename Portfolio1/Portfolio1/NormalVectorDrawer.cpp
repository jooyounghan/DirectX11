#include "NormalVectorDrawer.h"
#include "ID3D11Helper.h"
#include "DepthStencilState.h"
#include "SamplerState.h"
#include "ShaderTypeEnum.h"
#include <vector>

using namespace std;

NormalVectorDrawer::NormalVectorDrawer(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: LightlessDrawer(pDeviceIn, pDeviceContextIn)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(pDevice, L"BaseModelVS.hlsl", vInputElemDesc, cpNVVertexShader.GetAddressOf(), cpNVInputLayout.GetAddressOf());
	ID3D11Helper::CreateHS(pDevice, L"BaseModelHS.hlsl", cpNVHullShader.GetAddressOf());
	ID3D11Helper::CreateDS(pDevice, L"BaseModelDS.hlsl", cpNVDomainShader.GetAddressOf());
	ID3D11Helper::CreateGS(pDevice, L"NormalVectorGS.hlsl", cpNVGeometryShader.GetAddressOf());
	ID3D11Helper::CreatePS(pDevice, L"NormalVectorPS.hlsl", cpNVPixelShader.GetAddressOf());
}

NormalVectorDrawer::~NormalVectorDrawer()
{
}

void NormalVectorDrawer::SetIAInputLayer()
{
	pDeviceContext->IASetInputLayout(cpNVInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}

void NormalVectorDrawer::SetShader()
{
	pDeviceContext->VSSetShader(cpNVVertexShader.Get(), 0, 0);
	pDeviceContext->HSSetShader(cpNVHullShader.Get(), 0, 0);
	pDeviceContext->DSSetShader(cpNVDomainShader.Get(), 0, 0);
	pDeviceContext->GSSetShader(cpNVGeometryShader.Get(), 0, 0);
	pDeviceContext->PSSetShader(cpNVPixelShader.Get(), 0, 0);

	SamplerState& samplerState = SamplerState::GetInstance(pDevice);

	pDeviceContext->DSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
	pDeviceContext->GSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
}

void NormalVectorDrawer::SetOMState()
{
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}

void NormalVectorDrawer::ResetDrawer()
{
	pDeviceContext->PSSetShader(nullptr, 0, 0);
	pDeviceContext->GSSetShader(nullptr, 0, 0);
	pDeviceContext->DSSetShader(nullptr, 0, 0);
	pDeviceContext->HSSetShader(nullptr, 0, 0);
	pDeviceContext->VSSetShader(nullptr, 0, 0);

	ID3D11SamplerState* pResetSampler = nullptr;
	pDeviceContext->DSSetSamplers(SamplerType::WRAP_SAMPLER, 1, &pResetSampler);
	pDeviceContext->GSSetSamplers(SamplerType::WRAP_SAMPLER, 1, &pResetSampler);
	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, &pResetSampler);
}
