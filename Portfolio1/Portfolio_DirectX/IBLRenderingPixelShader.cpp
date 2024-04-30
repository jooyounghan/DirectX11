#include "IBLRenderingPixelShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "AIBLMesh.h"
#include "Viewable.h"

IBLRenderingPixelShader* IBLRenderingPixelShader::pPixelShader = nullptr;

using namespace std;

IBLRenderingPixelShader::IBLRenderingPixelShader()
	: IPixelShader()
{
	Console::Print("IBL Rendering Pixel Shader Generated.");
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

void IBLRenderingPixelShader::SetShader(void* pBindingSet)
{
	IBLPSBindingSet* pBinding = (IBLPSBindingSet*)pBindingSet;

	IImageFile* pImageFile = pBinding->pIblMesh->GetIBLTextureFile().get();
	ID3D11ShaderResourceView* pSRV = pImageFile != nullptr ? pImageFile->GetSRV() : nullptr;

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pBinding->pIblMesh->GetObjectBuffer());
	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
}

void IBLRenderingPixelShader::ResetShader()
{
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
}
