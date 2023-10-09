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

	ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"BaseModelVS.hlsl", vInputElemDesc, cpNVVertexShader.GetAddressOf(), cpNVInputLayout.GetAddressOf());
	ID3D11Helper::CreateHS(cpDevice.Get(), L"BaseModelHS.hlsl", cpNVHullShader.GetAddressOf());
	ID3D11Helper::CreateDS(cpDevice.Get(), L"BaseModelDS.hlsl", cpNVDomainShader.GetAddressOf());
	ID3D11Helper::CreateGS(cpDevice.Get(), L"NormalVectorGS.hlsl", cpNVGeometryShader.GetAddressOf());
	ID3D11Helper::CreatePS(cpDevice.Get(), L"NormalVectorPS.hlsl", cpNVPixelShader.GetAddressOf());
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, NULL, cpDevice.Get(), cpNVSampler.GetAddressOf());
}

NormalVectorDrawer::~NormalVectorDrawer()
{
}

void NormalVectorDrawer::SetIAInputLayer()
{
	cpDeviceContext->IASetInputLayout(cpNVInputLayout.Get());
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}

void NormalVectorDrawer::SetVSShader()
{
	cpDeviceContext->VSSetShader(cpNVVertexShader.Get(), 0, 0);
}

void NormalVectorDrawer::SetHSShader()
{
	cpDeviceContext->HSSetShader(cpNVHullShader.Get(), 0, 0);
}

void NormalVectorDrawer::SetDSShader()
{
	cpDeviceContext->DSSetShader(cpNVDomainShader.Get(), 0, 0);
}

void NormalVectorDrawer::SetGSShader()
{
	cpDeviceContext->GSSetShader(cpNVGeometryShader.Get(), 0, 0);
	cpDeviceContext->GSSetSamplers(0, 1, cpNVSampler.GetAddressOf());
}

void NormalVectorDrawer::SetPSShader()
{
	cpDeviceContext->PSSetShader(cpNVPixelShader.Get(), 0, 0);
	cpDeviceContext->PSSetSamplers(0, 1, cpNVSampler.GetAddressOf());
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
	cpDeviceContext->DSSetShader(nullptr, 0, 0);
	cpDeviceContext->HSSetShader(nullptr, 0, 0);
	cpDeviceContext->VSSetShader(nullptr, 0, 0);
}
