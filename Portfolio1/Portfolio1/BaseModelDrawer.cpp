#include "BaseModelDrawer.h"
#include "ID3D11Helper.h"
#include "DepthStencilState.h"
#include "ShaderTypeEnum.h"

#include <vector>

using namespace std;
using namespace Microsoft::WRL;

BaseModelDrawer::BaseModelDrawer(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: ObjectDrawer(cpDeviceIn, cpDeviceContextIn)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"BaseModelVS.hlsl", vInputElemDesc, cpBaseVertexShader.GetAddressOf(), cpBaseInputLayout.GetAddressOf());
	ID3D11Helper::CreateHS(cpDevice.Get(), L"BaseModelHS.hlsl", cpBaseHullShader.GetAddressOf());
	ID3D11Helper::CreateDS(cpDevice.Get(), L"BaseModelDS.hlsl", cpBaseDomainShader.GetAddressOf());
	ID3D11Helper::CreatePS(cpDevice.Get(), L"BaseModelPS.hlsl", cpBasePixelShader.GetAddressOf());
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, NULL, cpDevice.Get(), cpBaseWrapSampler.GetAddressOf());
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, NULL, cpDevice.Get(), cpBaseClampSampler.GetAddressOf());
}

BaseModelDrawer::~BaseModelDrawer()
{
}

void BaseModelDrawer::SetIAInputLayer()
{
	cpDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}

void BaseModelDrawer::SetVSShader()
{
	cpDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
}

void BaseModelDrawer::SetGSShader()
{
}

void BaseModelDrawer::SetHSShader()
{
	cpDeviceContext->HSSetShader(cpBaseHullShader.Get(), 0, 0);
}

void BaseModelDrawer::SetDSShader()
{
	cpDeviceContext->DSSetShader(cpBaseDomainShader.Get(), 0, 0);
	cpDeviceContext->DSSetSamplers(DSSamplerType::DS_WRAP_SAMPLER, 1, cpBaseWrapSampler.GetAddressOf());
	cpDeviceContext->DSSetSamplers(DSSamplerType::DS_CLAMP_SAMPLER, 1, cpBaseClampSampler.GetAddressOf());
}

void BaseModelDrawer::SetPSShader()
{
	cpDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);
	cpDeviceContext->PSSetSamplers(PSSamplerType::PS_WRAP_SAMPLER, 1, cpBaseWrapSampler.GetAddressOf());
	cpDeviceContext->PSSetSamplers(PSSamplerType::PS_CLAMP_SAMPLER, 1, cpBaseClampSampler.GetAddressOf());
}

void BaseModelDrawer::SetOMState()
{
	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::MaskOption), 1);
}

void BaseModelDrawer::ResetOMState()
{
	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DefaultOption), 0);
}

void BaseModelDrawer::ResetDrawer()
{
	cpDeviceContext->PSSetShader(nullptr, 0, 0);
	cpDeviceContext->DSSetShader(nullptr, 0, 0);
	cpDeviceContext->HSSetShader(nullptr, 0, 0);
	cpDeviceContext->VSSetShader(nullptr, 0, 0);

	ID3D11SamplerState* pResetSampler = nullptr;
	cpDeviceContext->DSSetSamplers(0, DSSamplerType::DS_WRAP_SAMPLER, &pResetSampler);
	cpDeviceContext->DSSetSamplers(0, DSSamplerType::DS_CLAMP_SAMPLER, &pResetSampler);

	cpDeviceContext->PSSetSamplers(PSSamplerType::PS_WRAP_SAMPLER, 1, &pResetSampler);
	cpDeviceContext->PSSetSamplers(PSSamplerType::PS_CLAMP_SAMPLER, 1, &pResetSampler);
}
