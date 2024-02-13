#include "BlurFilter.h"
#include "Shaders.h"
#include "DirectXDevice.h"

BlurFilter::BlurFilter(
	UINT uiWidthIn, 
	UINT uiHeightIn, 
	UINT uiArraySizeIn, 
	UINT uiNumQualityLevelsIn, 
	UINT uiBindFlagIn, 
	UINT uiCPUAccessIn, 
	UINT uiMiscFlagIn, 
	D3D11_USAGE eUsageIn, 
	DXGI_FORMAT eFormatIn
)
	: AFilter(
		uiWidthIn, uiHeightIn,
		uiArraySizeIn, uiNumQualityLevelsIn,
		uiBindFlagIn, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn,
		eFormatIn,
		256, 1, 1
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
}

BlurFilter::~BlurFilter()
{
}

void BlurFilter::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->CSSetShader(shaders.GetComputeShader(Shaders::BlurCS), NULL, NULL);
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppInputSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, cpUAV.GetAddressOf(), nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, DirectXDevice::ppClampSampler);

	DirectXDevice::pDeviceContext->Dispatch(
		uiWidth % uiThreadGroupCntX ? uiWidth / uiThreadGroupCntX + 1 : uiWidth / uiThreadGroupCntX,
		uiHeight % uiThreadGroupCntY ? uiHeight / uiThreadGroupCntY + 1 : uiHeight / uiThreadGroupCntY,
		uiThreadGroupCntZ);
	SetUAVBarrier();
}

void BlurFilter::SetUAVBarrier()
{
	ID3D11ShaderResourceView* pReleaseAndGetAddressOfSRV = nullptr;
	ID3D11UnorderedAccessView* pReleaseAndGetAddressOfUAV = nullptr;
	ID3D11SamplerState* pSampler = nullptr;

	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pReleaseAndGetAddressOfSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pReleaseAndGetAddressOfUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, &pSampler);
}
