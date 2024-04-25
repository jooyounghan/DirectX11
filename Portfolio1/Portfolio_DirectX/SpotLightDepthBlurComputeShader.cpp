#include "SpotLightDepthBlurComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

SpotLightDepthBlurComputeShader* SpotLightDepthBlurComputeShader::pComputeShader = nullptr;

SpotLightDepthBlurComputeShader::SpotLightDepthBlurComputeShader()
	: IComputeShader()
{
	Console::Print("Spot Light Depth Blur Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"SpotLightDepthBlurCS.hlsl", cpComputeShader.GetAddressOf());
}

SpotLightDepthBlurComputeShader::~SpotLightDepthBlurComputeShader()
{
}

void SpotLightDepthBlurComputeShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(cpComputeShader.Get(), NULL, NULL);
}

void SpotLightDepthBlurComputeShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
}

void SpotLightDepthBlurComputeShader::SetShader(ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV)
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
}

void SpotLightDepthBlurComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, &pNullSampler);
}