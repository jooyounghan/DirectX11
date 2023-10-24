#include "BlendFilter.h"
#include "ID3D11Helper.h"
#include "DefineVar.h"

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
	cpDeviceContext->PSSetShaderResources(0, 1, ppInput1SRV);
	cpDeviceContext->PSSetShaderResources(1, 1, ppInput2SRV);
	cpDeviceContext->PSSetConstantBuffers(0, 1, cpBlendConstantBuffer.GetAddressOf());
	cpDeviceContext->RSSetViewports(1, &sScreenViewport);

	cpDeviceContext->DrawIndexed((UINT)filterInfo.vIndices.size(), 0, 0);
}