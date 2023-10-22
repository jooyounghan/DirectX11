#include "BloomUp.h"
#include "ID3D11Helper.h"

BloomUpShader::BloomUpShader(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn)
	: cpDevice(cpDeviceIn)
{
	ID3D11Helper::CreateVSInputLayOut(
		cpDevice.Get(),
		L"BloomUpVS.hlsl",
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		},
		cpBloomUpVS.GetAddressOf(),
		cpBloomUpInputLayout.GetAddressOf()
	);

	ID3D11Helper::CreatePS(
		cpDevice.Get(),
		L"BloomUpPS.hlsl",
		cpBloomUpPS.GetAddressOf()
	);
}

BloomUpShader& BloomUpShader::GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn)
{
	static BloomUpShader s(cpDeviceIn);
	return s;
}

BloomUp::BloomUp(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn
)
	: FilterInterface(cpDeviceIn, cpDeviceContextIn, uiWidthIn, uiHeightIn)
{
	CreateOutputResource();
}

BloomUp::~BloomUp() {}

void BloomUp::CreateOutputResource()
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), uiWidth * 2, uiHeight * 2, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET , NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, cpOutputTexture2D.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpOutputTexture2D.Get(), cpOutputRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpOutputTexture2D.Get(), cpOutputSRV.GetAddressOf());
}

void BloomUp::StartFilter(ID3D11ShaderResourceView** ppInputSRV)
{
	UINT uiStride = sizeof(FilterVertex);
	UINT uiOffset = 0;
	FilterInfo& filterInfo = FilterInfo::GetIncetance(cpDevice);
	BloomUpShader& bloomUpShader = BloomUpShader::GetIncetance(cpDevice);

	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cpDeviceContext->IASetInputLayout(bloomUpShader.cpBloomUpInputLayout.Get());
	cpDeviceContext->IASetVertexBuffers(0, 1, filterInfo.cpVertexBuffer.GetAddressOf(), &uiStride, &uiOffset);
	cpDeviceContext->IASetIndexBuffer(filterInfo.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	cpDeviceContext->VSSetShader(bloomUpShader.cpBloomUpVS.Get(), NULL, NULL);

	cpDeviceContext->OMSetRenderTargets(1, cpOutputRTV.GetAddressOf(), NULL);

	cpDeviceContext->PSSetShader(bloomUpShader.cpBloomUpPS.Get(), NULL, NULL);
	cpDeviceContext->PSSetSamplers(0, 1, filterInfo.cpPSSamplerState.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(0, 1, ppInputSRV);

	cpDeviceContext->DrawIndexed(filterInfo.vIndices.size(), 0, 0);
}
