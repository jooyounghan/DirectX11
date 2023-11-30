#include "PostProcess.h"
#include "FilterInterface.h"
#include "ID3D11Helper.h"

#include "BloomFilter.h"
#include "BlendFilter.h"

PostProcess::PostProcess(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const D3D11_VIEWPORT& sScreenViewportIn,
	DXGI_FORMAT eCameraFormatIn,
	DXGI_FORMAT eBackBufferFormatIn
)
	: cpDevice(cpDeviceIn),cpDeviceContext(cpDeviceContextIn), sScreenViewport(sScreenViewportIn), uiDepth(1), eCameraFormat(eCameraFormatIn), eBackBufferFormat(eBackBufferFormatIn)
{
	pBlendFilter = new BlendFilter(cpDevice, cpDeviceContext, sScreenViewport, eBackBufferFormat);

	ID3D11Helper::CreateTexture2D(cpDevice.Get(), (UINT)sScreenViewport.Width, (UINT)sScreenViewport.Height, 1, 0, D3D11_BIND_SHADER_RESOURCE, NULL, NULL, D3D11_USAGE_DEFAULT, eCameraFormat, cpResolvedTexture.GetAddressOf());
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

	if (pBlendFilter != nullptr)
	{
		delete pBlendFilter;
		pBlendFilter = nullptr;
	}
}

void PostProcess::AddBloomFilter()
{
	UINT uiWidthOrigin = (UINT)sScreenViewport.Width;
	UINT uiHeightOrigin = (UINT)sScreenViewport.Height;

	UINT uiDepthForBloomUp = uiDepth;

	uiDepth *= 2;
	sScreenViewport.Width = (float)uiWidthOrigin / uiDepth;
	sScreenViewport.Height = (float)uiHeightOrigin / uiDepth;

	vBloomDownFilters.push_back(new BloomFilter(cpDevice, cpDeviceContext, sScreenViewport, eCameraFormat));

	sScreenViewport.Width = (float)uiWidthOrigin / (uiDepthForBloomUp);
	sScreenViewport.Height = (float)uiHeightOrigin / (uiDepthForBloomUp);
	vBloomUpFilters.push_front(new BloomFilter(cpDevice, cpDeviceContext, sScreenViewport, eCameraFormat));
}

void PostProcess::SetBlendProperties(const float& fBlendStrengthIn, const float& fExposureIn, const float& fGammaIn)
{
	if (pBlendFilter != nullptr)
	{
		pBlendFilter->SetBlendStrength(fBlendStrengthIn);
		pBlendFilter->SetExposure(fExposureIn);
		pBlendFilter->SetGamma(fGammaIn);
	}
}

void PostProcess::Update()
{
	for (auto& pFilter : vBloomDownFilters)
	{
		pFilter->Update();
	}
	for (auto& pFilter : vBloomUpFilters)
	{
		pFilter->Update();
	}
	pBlendFilter->Update();
}

void PostProcess::Process(
	ID3D11Texture2D* pStartTexture,
	ID3D11Texture2D* pBackBufferTexture,
	ID3D11RenderTargetView** ppBackBufferRTV
)
{
	cpDeviceContext->ResolveSubresource(cpResolvedTexture.Get(), 0, pStartTexture, 0, eCameraFormat);
	ID3D11Texture2D* pOutputTexture2D = cpResolvedTexture.Get();

	ID3D11ShaderResourceView** ppOriginOutputSRV = cpResolvedSRV.GetAddressOf();
	ID3D11ShaderResourceView** ppOutputSRV = ppOriginOutputSRV;

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

	if (pBlendFilter)
	{
		pBlendFilter->StartFilter(ppOutputSRV, ppOriginOutputSRV);
		pOutputTexture2D = pBlendFilter->GetOutputTexture2D();
	}

	cpDeviceContext->ResolveSubresource(pBackBufferTexture, 0, pOutputTexture2D, 0, eBackBufferFormat);
	cpDeviceContext->OMSetRenderTargets(1, ppBackBufferRTV, NULL);

}