#include "BloomDown.h"
#include "ID3D11Helper.h"

BloomDownShader::BloomDownShader(ComPtr<ID3D11Device>& cpDeviceIn)
	: cpDevice(cpDeviceIn)
{
	ID3D11Helper::CreateVSInputLayOut(
		cpDevice.Get(),
		L"BloomDownVS.hlsl",
		{ 
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		},
		cpBloomDownVS.GetAddressOf(),
		cpBloomDownInputLayout.GetAddressOf()
	);

	ID3D11Helper::CreatePS(
		cpDevice.Get(),
		L"BloomDownPS.hlsl",
		cpBloomDownPS.GetAddressOf()
	);
}

BloomDownShader& BloomDownShader::GetIncetance(ComPtr<ID3D11Device>& cpDeviceIn)
{
	static BloomDownShader s(cpDeviceIn);
	return s;
}

BloomDown::BloomDown(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn
)
	: FilterInterface(cpDeviceIn, cpDeviceContextIn, uiWidthIn, uiHeightIn)
{
	CreateOutputResource();
}

BloomDown::~BloomDown()
{
}

void BloomDown::CreateOutputResource()
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth / 2, uiHeight / 2, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, cpOutputTexture2D.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpOutputTexture2D.Get(), cpOutputRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpOutputTexture2D.Get(), cpOutputSRV.GetAddressOf());
}

void BloomDown::StartFilter(ID3D11ShaderResourceView** ppInputSRV)
{
	UINT uiStride = sizeof(FilterVertex);
	UINT uiOffset = 0;
	FilterInfo& filterInfo = FilterInfo::GetIncetance(cpDevice);
	BloomDownShader& bloomDownShader = BloomDownShader::GetIncetance(cpDevice);

	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cpDeviceContext->IASetInputLayout(bloomDownShader.cpBloomDownInputLayout.Get());
	cpDeviceContext->IASetVertexBuffers(0, 1, filterInfo.cpVertexBuffer.GetAddressOf(), &uiStride, &uiOffset);
	cpDeviceContext->IASetIndexBuffer(filterInfo.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	cpDeviceContext->VSSetShader(bloomDownShader.cpBloomDownVS.Get(), NULL, NULL);

	cpDeviceContext->OMSetRenderTargets(1, cpOutputRTV.GetAddressOf(), NULL);

	cpDeviceContext->PSSetShader(bloomDownShader.cpBloomDownPS.Get(), NULL, NULL);
	cpDeviceContext->PSSetSamplers(0, 1, filterInfo.cpPSSamplerState.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(0, 1, ppInputSRV);

	cpDeviceContext->DrawIndexed(filterInfo.vIndices.size(), 0, 0);
}
