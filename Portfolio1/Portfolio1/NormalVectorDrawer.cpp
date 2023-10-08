#include "NormalVectorDrawer.h"
#include "ID3D11Helper.h"
#include "DepthStencilState.h"
#include <vector>

NormalVectorDrawer::NormalVectorDrawer(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: DrawerInterface(cpDeviceIn, cpDeviceContextIn)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"NormalVectorVS.hlsl", vInputElemDesc, cpNVVertexShader.GetAddressOf(), cpNVInputLayout.GetAddressOf());
	ID3D11Helper::CreateGS(cpDevice.Get(), L"NormalVectorGS.hlsl", cpNVGeometryShader.GetAddressOf());
	ID3D11Helper::CreatePS(cpDevice.Get(), L"NormalVectorPS.hlsl", cpNVPixelShader.GetAddressOf());
}

NormalVectorDrawer::~NormalVectorDrawer()
{
}

void NormalVectorDrawer::SetIAInputLayer()
{
	cpDeviceContext->IASetInputLayout(cpNVInputLayout.Get());
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void NormalVectorDrawer::SetVSShader()
{
	cpDeviceContext->VSSetShader(cpNVVertexShader.Get(), 0, 0);
}

void NormalVectorDrawer::SetGSShader()
{
	cpDeviceContext->GSSetShader(cpNVGeometryShader.Get(), 0, 0);
}

void NormalVectorDrawer::SetHSShader()
{
}

void NormalVectorDrawer::SetDSShader()
{
}

void NormalVectorDrawer::SetPSShader()
{
	cpDeviceContext->PSSetShader(cpNVPixelShader.Get(), 0, 0);
}

void NormalVectorDrawer::SetOMState()
{
	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DefaultOption), 0);
}

void NormalVectorDrawer::ResetOMState()
{
}

void NormalVectorDrawer::ResetDrawer()
{
	cpDeviceContext->PSSetShader(nullptr, 0, 0);
	cpDeviceContext->GSSetShader(nullptr, 0, 0);
	cpDeviceContext->VSSetShader(nullptr, 0, 0);
}
