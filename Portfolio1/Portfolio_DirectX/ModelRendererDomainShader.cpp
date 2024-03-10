#include "ModelRendererDomainShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "PBRStaticMesh.h"
#include "Viewable.h"

using namespace std;

ModelRendererDomainShader::ModelRendererDomainShader()
{
	ID3D11Helper::CreateDS(
		DirectXDevice::pDevice, 
		L"PBRModelDS.hlsl", 
		cpDomainShader.GetAddressOf()
	);
}

ModelRendererDomainShader::~ModelRendererDomainShader()
{
}

void ModelRendererDomainShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->DSSetShader(cpDomainShader.Get(), NULL, NULL);
}

void ModelRendererDomainShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->DSSetShader(nullptr, NULL, NULL);
}

void ModelRendererDomainShader::SetShader(PBRStaticMesh& pbrStaticMesh, Viewable& viewable)
{
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, pbrStaticMesh.GetPBRConstantBuffer());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, viewable.GetViewProjBuffer());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, pbrStaticMesh.GetPBRTextureFlagBuffer());

	shared_ptr<IImageFile>& heightFile = pbrStaticMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
		heightFile.get() != nullptr ?
		heightFile->GetAddressOfSRV() : &pNullSRV
	);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
}

void ModelRendererDomainShader::ResetShader()
{
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, &pNullSampler);
}
