#include "TestModel.h"
#include "EnumBuffer.h"

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

	sModelTransformation.xmvTranslation.m128_f32[0] = fCenterX;
	sModelTransformation.xmvTranslation.m128_f32[1] = fCenterY;
	sModelTransformation.xmvTranslation.m128_f32[2] = fCenterZ;

	vector<Vertex> vVertex{
		{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f, fLen / 2.f},
		{fLen / 2.f, -fLen / 2.f, -fLen / 2.f, fLen / 2.f},
		{fLen / 2.f, fLen / 2.f, -fLen / 2.f, fLen / 2.f},
		{-fLen / 2.f, fLen / 2.f, -fLen / 2.f, fLen / 2.f},
		{-fLen / 2.f, -fLen / 2.f, fLen / 2.f, fLen / 2.f},
		{fLen / 2.f, -fLen / 2.f, fLen / 2.f, fLen / 2.f},
		{fLen / 2.f, fLen / 2.f, fLen / 2.f, fLen / 2.f},
		{-fLen / 2.f, fLen / 2.f, fLen / 2.f, fLen / 2.f},
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
	//cpDeviceContext->PSSetSamplers();
	//cpDeviceContext->PSSetConstantBuffers();

	//cpDeviceContext->RSSetState(cpRasterizerState.Get());

	//cpDeviceContext->OMSetDepthStencilState(cpDepthStencilState.Get(), 0);

	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}
