#include "ModelOutlineDrawer.h"
#include "ID3D11Helper.h"
#include "DepthStencilState.h"
#include "ModelInterface.h"

#include <vector>


ModelOutlineDrawer::ModelOutlineDrawer(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"BaseModelVS.hlsl", vInputElemDesc, cpBaseVertexShader.GetAddressOf(), cpBaseInputLayout.GetAddressOf());
	ID3D11Helper::CreatePS(cpDevice.Get(), L"OutlinerPS.hlsl", cpBasePixelShader.GetAddressOf());
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, NULL, cpDevice.Get(), cpBaseSampler.GetAddressOf());

}

ModelOutlineDrawer::~ModelOutlineDrawer()
{
}

void ModelOutlineDrawer::SetIAInputLayer()
{
	cpDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelOutlineDrawer::SetVSShader()
{
	cpDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
}

void ModelOutlineDrawer::SetHSShader()
{
}

void ModelOutlineDrawer::SetDSShader()
{
}

void ModelOutlineDrawer::SetPSShader()
{
	cpDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);
}

void ModelOutlineDrawer::SetOM()
{
	pModel->ScaleUp(0.1f, 0.1f, 0.1f);
	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DrawNotEqualOption), 1);
}

void ModelOutlineDrawer::ResetOM()
{
	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DefaultOption), 0);
	pModel->ScaleUp(-0.1f, -0.1f, -0.1f);
}

void ModelOutlineDrawer::ResetDrawer()
{
	cpDeviceContext->PSSetShader(nullptr, 0, 0);
	cpDeviceContext->VSSetShader(nullptr, 0, 0);
}

void ModelOutlineDrawer::SetModel(ModelInterface* modelInterface)
{
	pModel = modelInterface;
}
