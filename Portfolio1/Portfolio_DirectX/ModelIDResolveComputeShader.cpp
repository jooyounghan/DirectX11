#include "ModelIDResolveComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ModelIDResolveComputeShader* ModelIDResolveComputeShader::pComputeShader = nullptr;

ModelIDResolveComputeShader::ModelIDResolveComputeShader()
	: IComputeShader()
{
	Console::Print("Model ID Resolve Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"ModelIDResolveCS.hlsl", cpComputeShader.GetAddressOf());
}

ModelIDResolveComputeShader::~ModelIDResolveComputeShader()
{
}

void ModelIDResolveComputeShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(cpComputeShader.Get(), NULL, NULL);
}

void ModelIDResolveComputeShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
}

void ModelIDResolveComputeShader::SetShader(void* pBindingSet)
{
	SingleSourceCSBindingSet* pBinding = (SingleSourceCSBindingSet*)pBindingSet;
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, pBinding->ppInputSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, pBinding->ppOutputUAV, nullptr);
}

void ModelIDResolveComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, nullptr);
}
