#include "PointLightDepthBlurComputeShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

PointLightDepthBlurComputeShader* PointLightDepthBlurComputeShader::pComputeShader = nullptr;
ID3D11ShaderResourceView* PointLightDepthBlurComputeShader::pNullSRVs[6] = { nullptr };
ID3D11UnorderedAccessView* PointLightDepthBlurComputeShader::pNullUAVs[6] = { nullptr };

PointLightDepthBlurComputeShader::PointLightDepthBlurComputeShader()
	: IComputeShader()
{
	Console::Print("Point Light Depth Blur Compute Shader Generated.");
	ID3D11Helper::CreateCS(DirectXDevice::pDevice, L"PointLightDepthBlurCS.hlsl", cpComputeShader.GetAddressOf());
}

PointLightDepthBlurComputeShader::~PointLightDepthBlurComputeShader()
{
}

void PointLightDepthBlurComputeShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(cpComputeShader.Get(), NULL, NULL);
}

void PointLightDepthBlurComputeShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
}

void PointLightDepthBlurComputeShader::SetShader(void* pBindingSet)
{
	PointDepthBlurCSBindingSet* pBinding = (PointDepthBlurCSBindingSet*)pBindingSet;

	const size_t& uiCount = pBinding->uiIndex;

	DirectXDevice::pDeviceContext->CSSetShaderResources(0, uiCount, pBinding->ppInputSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, uiCount, pBinding->ppOutputUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
}

void PointLightDepthBlurComputeShader::ResetShader()
{
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 6, pNullSRVs);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 6, pNullUAVs, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, &pNullSampler);
}
