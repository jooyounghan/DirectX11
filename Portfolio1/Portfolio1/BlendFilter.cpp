#include "BlendFilter.h"
#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"

BlendShader::BlendShader(
	ID3D11Device* pDeviceIn
)
{
	ID3D11Helper::CreatePS(
		pDeviceIn,
		L"BlendPS.hlsl",
		cpBlendPS.GetAddressOf()
	);
}

BlendShader& BlendShader::GetIncetance(ID3D11Device* pDeviceIn)
{
	static BlendShader s(pDeviceIn);
	return s;
}

BlendFilter::BlendFilter(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn, 
	const D3D11_VIEWPORT& sScreenViewportIn, 
	DXGI_FORMAT eFormat
)
	: BlendFilterInterface(pDeviceIn, pDeviceContextIn, sScreenViewportIn, eFormat)
{
}

BlendFilter::~BlendFilter()
{
}

void BlendFilter::StartFilter(ID3D11ShaderResourceView** ppInput1SRV, ID3D11ShaderResourceView** ppInput2SRV)
{
	UINT uiStride = sizeof(FilterVertex);
	UINT uiOffset = 0;
	FilterInfo& filterInfo = FilterInfo::GetIncetance(pDevice);
	BlendShader& blendShader = BlendShader::GetIncetance(pDevice);

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->IASetInputLayout(filterInfo.cpFilterInputLayout.Get());
	pDeviceContext->IASetVertexBuffers(0, 1, filterInfo.cpVertexBuffer.GetAddressOf(), &uiStride, &uiOffset);
	pDeviceContext->IASetIndexBuffer(filterInfo.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->VSSetShader(filterInfo.cpFilterVS.Get(), NULL, NULL);

	pDeviceContext->OMSetRenderTargets(1, cpOutputRTV.GetAddressOf(), NULL);

	pDeviceContext->PSSetShader(blendShader.cpBlendPS.Get(), NULL, NULL);
	pDeviceContext->PSSetSamplers(0, 1, filterInfo.cpPSSamplerState.GetAddressOf());
	pDeviceContext->PSSetShaderResources(PS_SRV_FILTER1, 1, ppInput1SRV);
	pDeviceContext->PSSetShaderResources(PS_SRV_FILTER2, 1, ppInput2SRV);
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_BLEND_CONST, 1, cpBlendConstantBuffer.GetAddressOf());
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
	pDeviceContext->PSSetShaderResources(PS_SRV_FILTER2, 1, &pResetSRV);
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_BLEND_CONST, 1, &pResetBuffer);
}