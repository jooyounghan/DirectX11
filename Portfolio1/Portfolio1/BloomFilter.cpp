#include "BloomFilter.h"
#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"

BloomShader::BloomShader(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn)
{
	ID3D11Helper::CreatePS(
		cpDeviceIn.Get(),
		L"BloomPS.hlsl",
		cpBloomPS.GetAddressOf()
	);
}

BloomShader& BloomShader::GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn)
{
	static BloomShader s(cpDeviceIn);
	return s;
}

BloomFilter::BloomFilter(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const D3D11_VIEWPORT& sScreenViewportIn,
	DXGI_FORMAT eFormat
)
	: SingleFilterInterface(cpDeviceIn, cpDeviceContextIn, sScreenViewportIn, eFormat)
{

}

BloomFilter::~BloomFilter() 
{

}


void BloomFilter::StartFilter(ID3D11ShaderResourceView** ppInputSRV)
{
	UINT uiStride = sizeof(FilterVertex);
	UINT uiOffset = 0;
	FilterInfo& filterInfo = FilterInfo::GetIncetance(cpDevice);
	BloomShader& BloomShader = BloomShader::GetIncetance(cpDevice);

	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cpDeviceContext->IASetInputLayout(filterInfo.cpFilterInputLayout.Get());
	cpDeviceContext->IASetVertexBuffers(0, 1, filterInfo.cpVertexBuffer.GetAddressOf(), &uiStride, &uiOffset);
	cpDeviceContext->IASetIndexBuffer(filterInfo.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	cpDeviceContext->VSSetShader(filterInfo.cpFilterVS.Get(), NULL, NULL);

	cpDeviceContext->OMSetRenderTargets(1, cpOutputRTV.GetAddressOf(), NULL);

	cpDeviceContext->PSSetShader(BloomShader.cpBloomPS.Get(), NULL, NULL);
	cpDeviceContext->PSSetSamplers(0, 1, filterInfo.cpPSSamplerState.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(PS_SRV_FILTER1, 1, ppInputSRV);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_VIEWPORT, 1, cpViewportConstantBuffer.GetAddressOf());

	cpDeviceContext->RSSetViewports(1, &sScreenViewport);

	cpDeviceContext->DrawIndexed((UINT)filterInfo.vIndices.size(), 0, 0);

	ID3D11Buffer* pResetBuffer = nullptr;
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	ID3D11RenderTargetView* pResetRTV = nullptr;
	ID3D11SamplerState* pResetSampler = nullptr;

	cpDeviceContext->VSSetShader(nullptr, 0, 0);
	cpDeviceContext->PSSetShader(nullptr, 0, 0);
	cpDeviceContext->OMSetRenderTargets(1, &pResetRTV, NULL);

	cpDeviceContext->PSSetSamplers(0, 1, &pResetSampler);
	cpDeviceContext->PSSetShaderResources(PS_SRV_FILTER1, 1, &pResetSRV);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_VIEWPORT, 1, &pResetBuffer);
}
