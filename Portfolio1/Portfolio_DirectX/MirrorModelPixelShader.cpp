#include "MirrorModelPixelShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "MirrorModel.h"

MirrorModelPixelShader* MirrorModelPixelShader::pPixelShader = nullptr;

MirrorModelPixelShader::MirrorModelPixelShader()
	: IPixelShader()
{
	Console::Print("Mirror Model Pixel Shader Generated.");
	ID3D11Helper::CreatePS(
		DirectXDevice::pDevice,
		L"MirrorModelPS.hlsl",
		cpPixelShader.GetAddressOf()
	);
}

MirrorModelPixelShader::~MirrorModelPixelShader()
{
}

void MirrorModelPixelShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(cpPixelShader.Get(), NULL, NULL);
}

void MirrorModelPixelShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
}

void MirrorModelPixelShader::SetShader(MirrorModel& mirrorModel)
{
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, mirrorModel.GetIDBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, mirrorModel.GetMirrorPropertiesBuffer());

	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, mirrorModel.GetAddressOfSRV());
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
}

void MirrorModelPixelShader::ResetShader()
{
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
}
