#include "BloomFilter.h"
#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"
#include "RasterizationState.h"

BloomShader::BloomShader(ID3D11Device* pDeviceIn)
{
	ID3D11Helper::CreatePS(
		pDeviceIn,
		L"BloomPS.hlsl",
		cpBloomPS.GetAddressOf()
	);
}

BloomShader& BloomShader::GetIncetance(ID3D11Device* pDeviceIn)
{
	static BloomShader s(pDeviceIn);
	return s;
}

BloomFilter::BloomFilter(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const D3D11_VIEWPORT& sScreenViewportIn,
	DXGI_FORMAT eFormat
)
	: SingleFilterInterface(pDeviceIn, pDeviceContextIn, sScreenViewportIn, eFormat)
{

}

BloomFilter::~BloomFilter() 
{

}


void BloomFilter::StartFilter(ID3D11ShaderResourceView** ppInputSRV)
{
	UINT uiStride = sizeof(FilterVertex);
	UINT uiOffset = 0;
	FilterInfo& filterInfo = FilterInfo::GetIncetance(pDevice);
	BloomShader& BloomShader = BloomShader::GetIncetance(pDevice);

	RasterizationState& rasterizationState = RasterizationState::GetInstance(pDevice, pDeviceContext);
	pDeviceContext->RSSetState(rasterizationState.GetSolidRS());

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->IASetInputLayout(filterInfo.cpFilterInputLayout.Get());
	pDeviceContext->IASetVertexBuffers(0, 1, filterInfo.cpVertexBuffer.GetAddressOf(), &uiStride, &uiOffset);
	pDeviceContext->IASetIndexBuffer(filterInfo.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->VSSetShader(filterInfo.cpFilterVS.Get(), NULL, NULL);

	pDeviceContext->OMSetRenderTargets(1, cpOutputRTV.GetAddressOf(), NULL);

	pDeviceContext->PSSetShader(BloomShader.cpBloomPS.Get(), NULL, NULL);
	pDeviceContext->PSSetSamplers(0, 1, filterInfo.cpPSSamplerState.GetAddressOf());
	pDeviceContext->PSSetShaderResources(PS_SRV_FILTER1, 1, ppInputSRV);
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_VIEWPORT, 1, cpViewportConstantBuffer.GetAddressOf());

	pDeviceContext->RSSetViewports(1, &sScreenViewport);

	pDeviceContext->DrawIndexed((UINT)filterInfo.vIndices.size(), 0, 0);

	ID3D11Buffer* pResetBuffer = nullptr;
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	ID3D11RenderTargetView* pResetRTV = nullptr;
	ID3D11SamplerState* pResetSampler = nullptr;

	pDeviceContext->VSSetShader(nullptr, 0, 0);
	pDeviceContext->PSSetShader(nullptr, 0, 0);
	pDeviceContext->OMSetRenderTargets(1, &pResetRTV, NULL);

	pDeviceContext->PSSetSamplers(0, 1, &pResetSampler);
	pDeviceContext->PSSetShaderResources(PS_SRV_FILTER1, 1, &pResetSRV);
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_VIEWPORT, 1, &pResetBuffer);
}
