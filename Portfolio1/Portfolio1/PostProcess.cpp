#include "PostProcess.h"
#include "FilterInterface.h"
#include "ID3D11Helper.h"

PostProcess::PostProcess(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: cpDevice(cpDeviceIn),cpDeviceContext(cpDeviceContextIn)
{

}

PostProcess::~PostProcess()
{
	for (auto& pFilter : vFilters)
	{
		delete pFilter;
		pFilter = nullptr;
	}
	vFilters.clear();
}

void PostProcess::Process(
	ID3D11ShaderResourceView** ppStartSRV,
	ID3D11Texture2D* pBackBufferTexture,
	ID3D11RenderTargetView** ppBackBufferRTV
)
{
	ID3D11Texture2D* pOutputTexture2D = nullptr;
	ID3D11ShaderResourceView** ppOutputSRV = ppStartSRV;

	for (auto& pFilter : vFilters)
	{
		pFilter->StartFilter(ppOutputSRV);
		ppOutputSRV = pFilter->GetAddressOfOutputSRV();
		pOutputTexture2D = pFilter->GetOutputTexture2D();
	}

	cpDeviceContext->ResolveSubresource(pBackBufferTexture, 0, pOutputTexture2D, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
	cpDeviceContext->OMSetRenderTargets(1, ppBackBufferRTV, NULL);

}