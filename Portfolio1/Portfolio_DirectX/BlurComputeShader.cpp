#include "BlurComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

BlurComputeShader* BlurComputeShader::pComputeShader = nullptr;

BlurComputeShader::BlurComputeShader()
	: IComputeShader()
{
	Console::Print("Blur Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"BlurCS.hlsl", cpComputeShader.GetAddressOf());
}

BlurComputeShader::~BlurComputeShader()
{
}

void BlurComputeShader::ApplyShader()
{
}

void BlurComputeShader::DisapplyShader()
{
}

void BlurComputeShader::SetShader(ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV)
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
}

void BlurComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, &pNullSampler);
}