#include "BlendFilter.h"
#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"

BlendShader::BlendShader(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn
)
{
	ID3D11Helper::CreatePS(
		cpDeviceIn.Get(),
		L"BlendPS.hlsl",
		cpBlendPS.GetAddressOf()
	);
}

BlendShader& BlendShader::GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn)
{
	static BlendShader s(cpDeviceIn);
	return s;
}

BlendFilter::BlendFilter(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, 
	const D3D11_VIEWPORT& sScreenViewportIn, 
	DXGI_FORMAT eFormat
)
	: BlendFilterInterface(cpDeviceIn, cpDeviceContextIn, sScreenViewportIn, eFormat)
{
}

BlendFilter::~BlendFilter()
{
}

void BlendFilter::StartFilter(ID3D11ShaderResourceView** ppInput1SRV, ID3D11ShaderResourceView** ppInput2SRV)
{
	UINT uiStride = sizeof(FilterVertex);
	UINT uiOffset = 0;
	FilterInfo& filterInfo = FilterInfo::GetIncetance(cpDevice);
	BlendShader& blendShader = BlendShader::GetIncetance(cpDevice);

	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cpDeviceContext->IASetInputLayout(filterInfo.cpFilterInputLayout.Get());
	cpDeviceContext->IASetVertexBuffers(0, 1, filterInfo.cpVertexBuffer.GetAddressOf(), &uiStride, &uiOffset);
	cpDeviceContext->IASetIndexBuffer(filterInfo.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	cpDeviceContext->VSSetShader(filterInfo.cpFilterVS.Get(), NULL, NULL);

	cpDeviceContext->OMSetRenderTargets(1, cpOutputRTV.GetAddressOf(), NULL);

	cpDeviceContext->PSSetShader(blendShader.cpBlendPS.Get(), NULL, NULL);
	cpDeviceContext->PSSetSamplers(0, 1, filterInfo.cpPSSamplerState.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(PS_SRV_FILTER1, 1, ppInput1SRV);
	cpDeviceContext->PSSetShaderResources(PS_SRV_FILTER2, 1, ppInput2SRV);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_BLEND_CONST, 1, cpBlendConstantBuffer.GetAddressOf());
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
	cpDeviceContext->PSSetShaderResources(PS_SRV_FILTER2, 1, &pResetSRV);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_BLEND_CONST, 1, &pResetBuffer);
}