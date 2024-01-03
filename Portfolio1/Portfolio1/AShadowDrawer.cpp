#include "AShadowDrawer.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "PickableModelInterface.h"
#include "ID3D11Helper.h"
#include "DepthStencilState.h"

using namespace std;

AShadowDrawer::AShadowDrawer(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn
)
	: DrawerInterface(pDeviceIn, pDeviceContextIn)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(pDeviceIn, L"ShadowMapVS.hlsl", vInputElemDesc, cpShadowVertexShader.GetAddressOf(), cpShadowInputLayout.GetAddressOf());
	ID3D11Helper::CreatePS(pDevice, L"ShadowMapPS.hlsl", cpShadowPixelShader.GetAddressOf());
}

AShadowDrawer::~AShadowDrawer()
{
}


void AShadowDrawer::SetIAInputLayer()
{
	pDeviceContext->IASetInputLayout(cpShadowInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void AShadowDrawer::SetShader()
{
	pDeviceContext->VSSetShader(cpShadowVertexShader.Get(), 0, 0);
	pDeviceContext->PSSetShader(cpShadowPixelShader.Get(), 0, 0);
}

void AShadowDrawer::SetOMState()
{
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DepthOnlyOption), 0);
}

void AShadowDrawer::ResetDrawer()
{
	pDeviceContext->PSSetShader(nullptr, 0, 0);
	pDeviceContext->VSSetShader(nullptr, 0, 0);

	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);

	ID3D11RenderTargetView* resetRTV = nullptr;
	pDeviceContext->OMSetRenderTargets(1, &resetRTV, nullptr);
}
