#include "ModelDrawer.h"
#include "ID3D11Helper.h"

using namespace std;
using namespace Microsoft::WRL;

template<typename Model>
class ModelDrawerImpl
{
public:
	template<typename ...Args>
	ModelDrawerImpl(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, Args... args);

protected:
	Model templateModel;

public:
	void Update();
	void Render();
	void RenderOutline();

protected:
	ComPtr<ID3D11Device>& cpDevice;
	ComPtr<ID3D11DeviceContext>& cpDeviceContext;

protected:
	ComPtr<ID3D11InputLayout>	cpBaseInputLayout;
	ComPtr<ID3D11VertexShader>	cpBaseVertexShader;

protected:
	ComPtr<ID3D11PixelShader>	cpBasePixelShader;
	ComPtr<ID3D11SamplerState>	cpBaseSampler;

protected:
	ComPtr<ID3D11HullShader>		cpBaseHullShader;
	ComPtr<ID3D11DomainShader>	cpBaseDomainShader;

};

template<typename Model>
template<typename ...Args>
ModelDrawerImpl<Model>::ModelDrawerImpl(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, Args ...args)
	: templateModel(cpDeviceIn, cpDeviceContextIn, args...)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
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

template<typename Model>
void ModelDrawerImpl<Model>::Update()
{
	templateModel.Update();
}

template<typename Model>
void ModelDrawerImpl<Model>::Render()
{
	cpDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	cpDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
	templateModel.SetVSConstantBuffer();
	templateModel.SetVSShaderResources();

	cpDeviceContext->HSSetShader(cpBaseHullShader.Get(), 0, 0);
	templateModel.SetHSConstantBuffer();
	templateModel.SetHSShaderResources();

	cpDeviceContext->DSSetShader(cpBaseDomainShader.Get(), 0, 0);
	cpDeviceContext->DSSetSamplers(0, 1, cpBaseSampler.GetAddressOf());
	templateModel.SetDSConstantBuffer();
	templateModel.SetDSShaderResources();

	cpDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);
	cpDeviceContext->PSSetSamplers(0, 1, cpBaseSampler.GetAddressOf());
	templateModel.SetPSConstantBuffer();
	templateModel.SetPSShaderResources();

	templateModel.SetOM();

	templateModel.Render();
	
	templateModel.ResetOM();

	cpDeviceContext->PSSetShader(nullptr, 0, 0);
	cpDeviceContext->DSSetShader(nullptr, 0, 0);
	cpDeviceContext->HSSetShader(nullptr, 0, 0);
	cpDeviceContext->VSSetShader(nullptr, 0, 0);
}

template<typename Model>
void ModelDrawerImpl<Model>::RenderOutline()
{
	templateModel.RenderOutline();
}
