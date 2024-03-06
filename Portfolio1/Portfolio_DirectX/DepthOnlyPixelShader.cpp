#include "DepthOnlyPixelShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

DepthOnlyPixelShader::DepthOnlyPixelShader()
	: IPixelShader()
{
	ID3D11Helper::CreatePS(
		DirectXDevice::pDevice,
		L"DepthOnlyPathPS.hlsl",
		cpPixelShader.GetAddressOf()
	);
}

DepthOnlyPixelShader::~DepthOnlyPixelShader()
{
}

void DepthOnlyPixelShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(cpPixelShader.Get(), NULL, NULL);
}

void DepthOnlyPixelShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
}

void DepthOnlyPixelShader::SetShader()
{
}

void DepthOnlyPixelShader::ResetShader()
{
}
