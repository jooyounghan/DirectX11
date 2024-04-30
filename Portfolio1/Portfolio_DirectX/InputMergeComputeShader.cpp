#include "InputMergeComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

InputMergeComputeShader* InputMergeComputeShader::pComputeShader = nullptr;

InputMergeComputeShader::InputMergeComputeShader()
	: IComputeShader()
{
	Console::Print("Input Merge Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"InputMergeCS.hlsl", cpComputeShader.GetAddressOf());
}

InputMergeComputeShader::~InputMergeComputeShader()
{
}

void InputMergeComputeShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(cpComputeShader.Get(), NULL, NULL);
}

void InputMergeComputeShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
}

void InputMergeComputeShader::SetShader(void* pBindingSet)
{
	DoubleSourceCSBindingSet* pBinding = (DoubleSourceCSBindingSet*)pBindingSet;
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, pBinding->ppInputSRV1);
	DirectXDevice::pDeviceContext->CSSetShaderResources(1, 1, pBinding->ppInputSRV2);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, pBinding->ppOutputUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
}

void InputMergeComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetShaderResources(1, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, &pNullSampler);
}
