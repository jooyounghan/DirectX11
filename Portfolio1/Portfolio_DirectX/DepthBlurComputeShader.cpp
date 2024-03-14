#include "DepthBlurComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

DepthBlurComputeShader* DepthBlurComputeShader::pComputeShader = nullptr;

DepthBlurComputeShader::DepthBlurComputeShader()
	: IComputeShader()
{
	Console::Print("Depth Blur Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"DepthBlurCS.hlsl", cpComputeShader.GetAddressOf());
}

DepthBlurComputeShader::~DepthBlurComputeShader()
{
}

void DepthBlurComputeShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(cpComputeShader.Get(), NULL, NULL);
}

void DepthBlurComputeShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
}

void DepthBlurComputeShader::SetShader(ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV)
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
}

void DepthBlurComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, &pNullSampler);
}