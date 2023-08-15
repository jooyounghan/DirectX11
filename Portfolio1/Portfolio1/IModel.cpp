#include "IModel.h"
#include "ID3D11Helper.h"
#include <atomic>

std::atomic_bool IModel::bBaseInitialized = false;
ComPtr<ID3D11InputLayout>	IModel::cpBaseInputLayout;
ComPtr<ID3D11VertexShader>	IModel::cpBaseVertexShader;
ComPtr<ID3D11PixelShader>	IModel::cpBasePixelShader;

struct Vertex
{
	float x, y, z, w;
};

IModel::IModel(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
	if (!bBaseInitialized.load())
	{
		vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"BaseModelVS.hlsl", vInputElemDesc, cpBaseVertexShader.GetAddressOf(), cpBaseInputLayout.GetAddressOf());
		ID3D11Helper::CreatePS(cpDevice.Get(), L"BaseModelPS.hlsl", cpBasePixelShader.GetAddressOf());

		bBaseInitialized.store(true);
	}
}

void IModel::Render()
{
	vector<uint32_t> vIndex{
		0, 3, 1,
		1, 3, 2,
		1, 2, 5,
		2, 6, 5,
		0, 7, 3,
		0, 4, 7,
		4, 5, 7,
		7, 5, 6,
		0, 1, 5,
		0, 5, 4,
		3, 7, 2,
		2, 7, 6
	};
	ui32IndexCount = UINT(vIndex.size());
	vector<Vertex> vVertex{
		{-0.1f, -0.1f, 0.0f, 1.0f},
		{0.1f, -0.1f, 0.0f, 1.0f},
		{0.1f, 0.1f, 0.0f, 1.0f},
		{-0.1f, 0.1f, 0.0f, 1.0f},
		{-0.1f, -0.1f, 0.1f, 1.0f},
		{0.1f, -0.1f, 0.1f, 1.0f},
		{0.1f, 0.1f, 0.1f, 1.0f},
		{-0.1f, 0.1f, 0.1f, 1.0f},
	};

	ID3D11Helper::CreateBuffer(cpDevice.Get(), vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, cpIndexBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(cpDevice.Get(), vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, cpVertexBuffer.GetAddressOf());

	cpDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cpDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
	//cpDeviceContext->VSSetSamplers();
	//cpDeviceContext->VSSetConstantBuffers();

	cpDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);
	//cpDeviceContext->PSSetSamplers();
	//cpDeviceContext->PSSetConstantBuffers();

	//cpDeviceContext->RSSetState(cpRasterizerState.Get());

	//cpDeviceContext->OMSetDepthStencilState(cpDepthStencilState.Get(), 0);

	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}
