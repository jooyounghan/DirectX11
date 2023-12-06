#include "CubeMapDrawer.h"
#include "ID3D11Helper.h"
#include "CubeMapModel.h"

#include "DepthStencilState.h"
#include "SamplerState.h"
#include "ShaderTypeEnum.h"

#include <vector>

using namespace std;
using namespace Microsoft::WRL;

CubeMapDrawer::CubeMapDrawer(
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

		ID3D11Helper::CreateVSInputLayOut(pDevice, L"CubeMapVS.hlsl", vInputElemDesc, cpCubeMapVertexShader.GetAddressOf(), cpCubeMapInputLayout.GetAddressOf());
		ID3D11Helper::CreatePS(pDevice, L"CubeMapPS.hlsl", cpCubeMapPixelShader.GetAddressOf());
}

CubeMapDrawer::~CubeMapDrawer()
{
}

void CubeMapDrawer::SetIAInputLayer()
{
	pDeviceContext->IASetInputLayout(cpCubeMapInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CubeMapDrawer::SetShader()
{
	pDeviceContext->VSSetShader(cpCubeMapVertexShader.Get(), 0, 0);
	pDeviceContext->PSSetShader(cpCubeMapPixelShader.Get(), 0, 0);

	SamplerState& samplerState = SamplerState::GetInstance(pDevice);
	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
	pDeviceContext->PSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, samplerState.GetAddressOfClampSampler());
}

void CubeMapDrawer::SetOMState()
{
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}


void CubeMapDrawer::ResetDrawer()
{
	pDeviceContext->PSSetShader(nullptr, 0, 0);
	pDeviceContext->VSSetShader(nullptr, 0, 0);

	ID3D11SamplerState* pResetSampler = nullptr;
	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, &pResetSampler);
	pDeviceContext->PSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, &pResetSampler);
}
