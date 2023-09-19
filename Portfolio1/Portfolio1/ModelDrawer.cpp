#include "ModelDrawer.h"

ModelDrawer::ModelDrawer(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"BaseModelVS.hlsl", vInputElemDesc, cpBaseVertexShader.GetAddressOf(), cpBaseInputLayout.GetAddressOf());
	ID3D11Helper::CreateHS(cpDevice.Get(), L"BaseModelHS.hlsl", cpBaseHullShader.GetAddressOf());
	ID3D11Helper::CreateDS(cpDevice.Get(), L"BaseModelDS.hlsl", cpBaseDomainShader.GetAddressOf());
	ID3D11Helper::CreatePS(cpDevice.Get(), L"BaseModelPS.hlsl", cpBasePixelShader.GetAddressOf());
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, NULL, cpDevice.Get(), cpBaseSampler.GetAddressOf());

}

ModelDrawer::~ModelDrawer()
{
}

void ModelDrawer::SetIAInputLayer()
{
	cpDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
}

void ModelDrawer::SetVSShader()
{
	cpDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
}

void ModelDrawer::SetHSShader()
{
	cpDeviceContext->HSSetShader(cpBaseHullShader.Get(), 0, 0);
}

void ModelDrawer::SetDSShader()
{
	cpDeviceContext->DSSetShader(cpBaseDomainShader.Get(), 0, 0);
	cpDeviceContext->DSSetSamplers(0, 1, cpBaseSampler.GetAddressOf());
}

void ModelDrawer::SetPSShader()
{
	cpDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);
	cpDeviceContext->PSSetSamplers(0, 1, cpBaseSampler.GetAddressOf());
}

void ModelDrawer::ResetDrawer()
{
	cpDeviceContext->PSSetShader(nullptr, 0, 0);
	cpDeviceContext->DSSetShader(nullptr, 0, 0);
	cpDeviceContext->HSSetShader(nullptr, 0, 0);
	cpDeviceContext->VSSetShader(nullptr, 0, 0);
}
