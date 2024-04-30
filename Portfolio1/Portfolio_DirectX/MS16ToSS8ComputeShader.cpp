#include "MS16ToSS8ComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

MS16ToSS8ComputeShader* MS16ToSS8ComputeShader::pComputeShader = nullptr;

MS16ToSS8ComputeShader::MS16ToSS8ComputeShader()
	: IComputeShader()
{
	Console::Print("MS16ToSS8 Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"MS16ToSS8CS.hlsl", cpComputeShader.GetAddressOf());
}

MS16ToSS8ComputeShader::~MS16ToSS8ComputeShader()
{
}

void MS16ToSS8ComputeShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(cpComputeShader.Get(), NULL, NULL);
}

void MS16ToSS8ComputeShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
}

void MS16ToSS8ComputeShader::SetShader(void* pBindingSet)
{
	SingleSourceCSBindingSet* pBinding = (SingleSourceCSBindingSet*)pBindingSet;
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, pBinding->ppInputSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, pBinding->ppOutputUAV, nullptr);
}

void MS16ToSS8ComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, nullptr);
}

