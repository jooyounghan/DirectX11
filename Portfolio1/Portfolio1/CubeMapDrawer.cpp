#include "CubeMapDrawer.h"
#include "ID3D11Helper.h"
#include "DepthStencilState.h"

#include "CubeMapModel.h"

#include <vector>

using namespace std;
using namespace Microsoft::WRL;

CubeMapDrawer::CubeMapDrawer(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: NonLightDrawer(cpDeviceIn, cpDeviceContextIn)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

		ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"CubeMapVS.hlsl", vInputElemDesc, cpCubeMapVertexShader.GetAddressOf(), cpCubeMapInputLayout.GetAddressOf());
		ID3D11Helper::CreatePS(cpDevice.Get(), L"CubeMapPS.hlsl", cpCubeMapPixelShader.GetAddressOf());
		ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, NULL, cpDevice.Get(), cpCubeMapSampler.GetAddressOf());
}

CubeMapDrawer::~CubeMapDrawer()
{
}

void CubeMapDrawer::Draw(CameraInterface* pCamera, CubeMapModel* pCubeMap)
{
	NonLightDrawer::Draw(pCamera, pCubeMap);
}

void CubeMapDrawer::SetIAInputLayer()
{
	cpDeviceContext->IASetInputLayout(cpCubeMapInputLayout.Get());
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CubeMapDrawer::SetVSShader()
{
	cpDeviceContext->VSSetShader(cpCubeMapVertexShader.Get(), 0, 0);
}

void CubeMapDrawer::SetHSShader()
{
}

void CubeMapDrawer::SetDSShader()
{
}

void CubeMapDrawer::SetGSShader()
{
}

void CubeMapDrawer::SetPSShader()
{
	cpDeviceContext->PSSetShader(cpCubeMapPixelShader.Get(), 0, 0);
	cpDeviceContext->PSSetSamplers(0, 1, cpCubeMapSampler.GetAddressOf());
}

void CubeMapDrawer::SetOMState()
{
	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DefaultOption), 0);
}

void CubeMapDrawer::ResetOMState()
{
}

void CubeMapDrawer::ResetDrawer()
{
	cpDeviceContext->PSSetShader(nullptr, 0, 0);
	cpDeviceContext->VSSetShader(nullptr, 0, 0);

	ID3D11SamplerState* pResetSampler = nullptr;
	cpDeviceContext->PSSetSamplers(0, 1, &pResetSampler);
}
