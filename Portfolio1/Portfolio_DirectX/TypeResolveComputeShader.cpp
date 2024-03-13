#include "TypeResolveComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

TypeResolveComputeShader* TypeResolveComputeShader::pComputeShader = nullptr;

TypeResolveComputeShader::TypeResolveComputeShader()
	: IComputeShader()
{
	Console::Print("Type Resolve Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"TypeResolveCS.hlsl", cpComputeShader.GetAddressOf());
}


TypeResolveComputeShader::~TypeResolveComputeShader()
{
}

void TypeResolveComputeShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(cpComputeShader.Get(), NULL, NULL);
}

void TypeResolveComputeShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
}

void TypeResolveComputeShader::SetShader(ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV)
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAV, nullptr);
}

void TypeResolveComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV , nullptr);
}
