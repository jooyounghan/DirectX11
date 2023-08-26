#include "TestModel.h"
#include "EnumVar.h"

#include <atomic>

using namespace std;

atomic_bool					TestModel::bBaseInitialized = false;
ComPtr<ID3D11InputLayout>	TestModel::cpBaseInputLayout;
ComPtr<ID3D11VertexShader>	TestModel::cpBaseVertexShader;
ComPtr<ID3D11PixelShader>	TestModel::cpBasePixelShader;
ComPtr<ID3D11SamplerState>	TestModel::cpBaseSampler;

TestModel::TestModel(
	ComPtr<ID3D11Device>& cpDeviceIn,
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fLen
)
	: IModel(cpDeviceIn, cpDeviceContextIn)
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

		FLOAT pBorderColor[4]{ 0.f, 0.f, 0.f, 0.f };
		ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, pBorderColor, cpDevice.Get(), cpBaseSampler.GetAddressOf());
	
		bBaseInitialized.store(true);
	}

	vector<uint32_t> vIndex{
		0, 1, 2,
		3, 4, 5,

		6, 7, 8,
		9, 10, 11,
		
		12, 13, 14,
		15, 16, 17,

		18, 19, 20,
		21, 22, 23,

		24, 25, 26,
		27, 28, 29,

		30, 31, 32,
		33, 34, 35,
	};
	ui32IndexCount = UINT(vIndex.size());

	sModelTransformation.xmvTranslation.m128_f32[0] = fCenterX;
	sModelTransformation.xmvTranslation.m128_f32[1] = fCenterY;
	sModelTransformation.xmvTranslation.m128_f32[2] = fCenterZ;

	vector<Vertex> vVertex{
		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, -1.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, -1.f}},
		{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 1.f},  {0.f, 0.f, -1.f}},
		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f},  {0.f, 0.f, -1.f}},
		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f},  {0.f, 0.f, -1.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f},  {0.f, 0.f, -1.f}},


		{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {1.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {1.f, 0.f, 0.f}},


		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {-1.f, 0.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {-1.f, 0.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f}},


		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {0.f, -1.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f, -1.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f, -1.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f, -1.f, 0.f}},


		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, -1.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, -1.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, -1.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, -1.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 1.f}, {0.f, -1.f, 0.f}},


		{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f ,0.f, 1.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f ,0.f, 1.f}},
		{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f ,0.f, 1.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f ,0.f, 1.f}},
		{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {0.f ,0.f, 1.f}},
		{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f ,0.f, 1.f}}
	};

	ID3D11Helper::CreateBuffer(cpDevice.Get(), vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, cpIndexBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(cpDevice.Get(), vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, cpVertexBuffer.GetAddressOf());

	cpDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
}

void TestModel::Update()
{
	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		ModelTransform::GetAffineTransformMatrix(sModelTransformation),
		D3D11_MAP_WRITE_DISCARD,
		cpModelMatrixBuffer.Get()
	);
}

void TestModel::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cpDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
	cpDeviceContext->VSSetConstantBuffers(VSConstBuffer::ModelMatrix, 1, cpModelMatrixBuffer.GetAddressOf());
	//cpDeviceContext->VSSetSamplers();
	//cpDeviceContext->VSSetConstantBuffers();

	cpDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);
	cpDeviceContext->PSSetSamplers(0, 1, cpBaseSampler.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(0, 1, cpShaderResourceViewTexture.GetAddressOf());
	//cpDeviceContext->PSSetConstantBuffers();

	//cpDeviceContext->RSSetState(cpRasterizerState.Get());

	//cpDeviceContext->OMSetDepthStencilState(cpDepthStencilState.Get(), 0);

	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}
