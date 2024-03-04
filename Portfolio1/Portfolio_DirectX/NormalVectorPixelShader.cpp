#include "NormalVectorPixelShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

NormalVectorPixelShader::NormalVectorPixelShader()
	: IPixelShader()
{
	ID3D11Helper::CreatePS(
		DirectXDevice::pDevice, 
		L"NormalVectorPS.hlsl", 
		cpPixelShader.GetAddressOf()
	);
}

NormalVectorPixelShader::~NormalVectorPixelShader()
{
}

void NormalVectorPixelShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(cpPixelShader.Get(), NULL, NULL);
}

void NormalVectorPixelShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
}

void NormalVectorPixelShader::SetShader()
{
}

void NormalVectorPixelShader::ResetShader()
{
}
