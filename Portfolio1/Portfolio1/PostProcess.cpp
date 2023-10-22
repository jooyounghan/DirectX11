#include "PostProcess.h"
#include "FilterInterface.h"
#include "ID3D11Helper.h"

#include "BloomDown.h"
#include "BloomUp.h"
#include <stack>
PostProcess::PostProcess(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const D3D11_VIEWPORT& sScreenViewportIn,
	DXGI_FORMAT eFormatIn
)
	: cpDevice(cpDeviceIn),cpDeviceContext(cpDeviceContextIn), sScreenViewport(sScreenViewportIn), uiDepth(1), eFormat(eFormatIn)
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), sScreenViewport.Width, sScreenViewport.Height, 1, 0, D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, eFormat, cpResolvedTexture.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpResolvedTexture.Get(), cpResolvedSRV.GetAddressOf());
}

PostProcess::~PostProcess()
{
	for (auto& pFilter : vBloomDownFilters)
	{
		delete pFilter;
		pFilter = nullptr;
	}
	vBloomDownFilters.clear();

	for (auto& pFilter : vBloomUpFilters)
	{
		delete pFilter;
		pFilter = nullptr;
	}
	vBloomUpFilters.clear();
}

void PostProcess::AddBloomFilter()
{
	UINT uiWidthOrigin = sScreenViewport.Width;
	UINT uiHeightOrigin = sScreenViewport.Height;
	sScreenViewport.Width = uiWidthOrigin / uiDepth;
	sScreenViewport.Height = uiHeightOrigin / uiDepth;
	vBloomDownFilters.push_back(new BloomDown(cpDevice, cpDeviceContext, sScreenViewport, eFormat));

	sScreenViewport.Width = uiWidthOrigin / (2 * uiDepth);
	sScreenViewport.Height = uiHeightOrigin / (2 * uiDepth);
	vBloomUpFilters.push_front(new BloomUp(cpDevice, cpDeviceContext, sScreenViewport, eFormat));

	uiDepth *= 2;
}

void PostProcess::Process(
	ID3D11Texture2D* pStartTexture,
	ID3D11Texture2D* pBackBufferTexture,
	ID3D11RenderTargetView** ppBackBufferRTV
)
{
	cpDeviceContext->ResolveSubresource(cpResolvedTexture.Get(), 0, pStartTexture, 0, eFormat);
	ID3D11Texture2D* pOutputTexture2D = nullptr;
	ID3D11ShaderResourceView** ppOutputSRV = cpResolvedSRV.GetAddressOf();

	for (auto& pFilter : vBloomDownFilters)
	{
		pFilter->StartFilter(ppOutputSRV);
		ppOutputSRV = pFilter->GetAddressOfOutputSRV();
		pOutputTexture2D = pFilter->GetOutputTexture2D();
	}

	for (auto& pFilter : vBloomUpFilters)
	{
		pFilter->StartFilter(ppOutputSRV);
		ppOutputSRV = pFilter->GetAddressOfOutputSRV();
		pOutputTexture2D = pFilter->GetOutputTexture2D();
	}

	cpDeviceContext->ResolveSubresource(pBackBufferTexture, 0, pOutputTexture2D, 0, eFormat);
	cpDeviceContext->OMSetRenderTargets(1, ppBackBufferRTV, NULL);

}