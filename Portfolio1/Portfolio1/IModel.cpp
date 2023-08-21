#include "IModel.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include <atomic>

using namespace DirectX;

std::atomic_bool IModel::bBaseInitialized = false;
ComPtr<ID3D11InputLayout>	IModel::cpBaseInputLayout;
ComPtr<ID3D11VertexShader>	IModel::cpBaseVertexShader;
ComPtr<ID3D11PixelShader>	IModel::cpBasePixelShader;

IModel::IModel(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
	if (!bBaseInitialized.load())
	{
		vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"BaseModelVS.hlsl", vInputElemDesc, cpBaseVertexShader.GetAddressOf(), cpBaseInputLayout.GetAddressOf());
		ID3D11Helper::CreatePS(cpDevice.Get(), L"BaseModelPS.hlsl", cpBasePixelShader.GetAddressOf());

		bBaseInitialized.store(true);
	}

	ModelTransform::Init(&sModelTransformation);

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		ModelTransform::GetAffineTransformMatrix(sModelTransformation),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpModelMatrixBuffer.GetAddressOf()
	);
}