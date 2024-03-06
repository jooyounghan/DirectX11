#include "IBLRenderingPixelShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

IBLRenderingPixelShader::IBLRenderingPixelShader()
	: IPixelShader()
{
	ID3D11Helper::CreatePS(
		DirectXDevice::pDevice,
		L"IBLModelPS.hlsl",
		cpPixelShader.GetAddressOf()
	);
}

IBLRenderingPixelShader::~IBLRenderingPixelShader()
{
}

void IBLRenderingPixelShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(cpPixelShader.Get(), NULL, NULL);
}

void IBLRenderingPixelShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
}

void IBLRenderingPixelShader::SetShader()
{

}

void IBLRenderingPixelShader::ResetShader()
{
}
