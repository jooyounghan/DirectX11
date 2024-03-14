#include "ColorBlurComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ColorBlurComputeShader* ColorBlurComputeShader::pComputeShader = nullptr;

ColorBlurComputeShader::ColorBlurComputeShader()
	: IComputeShader()
{
	Console::Print("Color Blur Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"ColorBlurCS.hlsl", cpComputeShader.GetAddressOf());
}

ColorBlurComputeShader::~ColorBlurComputeShader()
{
}

void ColorBlurComputeShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(cpComputeShader.Get(), NULL, NULL);
}

void ColorBlurComputeShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
}

void ColorBlurComputeShader::SetShader(ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV)
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
}

void ColorBlurComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, &pNullSampler);
}