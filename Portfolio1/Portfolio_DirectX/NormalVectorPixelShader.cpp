#include "NormalVectorPixelShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

NormalVectorPixelShader* NormalVectorPixelShader::pPixelShader = nullptr;

NormalVectorPixelShader::NormalVectorPixelShader()
	: IPixelShader()
{
	Console::Print("Normal Vector Pixel Shader Generated.");
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

void NormalVectorPixelShader::SetShader(void* pBindingSet)
{
}

void NormalVectorPixelShader::ResetShader()
{
}
