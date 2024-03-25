#include "PBRIBLLightPixelShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "AIBLMesh.h"
#include "PBRStaticMesh.h"
#include "Viewable.h"

PBRIBLLightPixelShader* PBRIBLLightPixelShader::pPixelShader = nullptr;

PBRIBLLightPixelShader::PBRIBLLightPixelShader()
	: IPixelShader()
{
	Console::Print("PBR IBL Light Pixel Shader Generated.");
	ID3D11Helper::CreatePS(
		DirectXDevice::pDevice,
		L"PBRWithIBLPS.hlsl",
		cpPixelShader.GetAddressOf()
	);
}

PBRIBLLightPixelShader::~PBRIBLLightPixelShader()
{
}

void PBRIBLLightPixelShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(cpPixelShader.Get(), NULL, NULL);
	//DirectXDevice::pDeviceContext->OMSetBlendState(DirectXDevice::pAddingBlendState, NULL, UINT(0xFFFFFFFF));
	//DirectXDevice::pDeviceContext->OMSetDepthStencilState(DirectXDevice::pDrawLessEqualDSS, 0);
}

void PBRIBLLightPixelShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	//DirectXDevice::pDeviceContext->OMSetBlendState(nullptr, NULL, UINT(0xFFFFFFFF));
	//DirectXDevice::pDeviceContext->OMSetDepthStencilState(nullptr, 0);
}

void PBRIBLLightPixelShader::SetShader(AIBLMesh& iblMesh, IObject& idObject, PBRStaticMesh& pbrStaticMesh, Viewable& viewable)
{
	IImageFile* pImageFile = nullptr;
	ID3D11ShaderResourceView* pSRV = nullptr;
	auto SetSRV = [&](IImageFile* pImage, const UINT& startSlot, const UINT& numViews) {
		pImageFile = pImage;
		pSRV = pImageFile != nullptr ? pImageFile->GetSRV() : nullptr;
		DirectXDevice::pDeviceContext->PSSetShaderResources(startSlot, numViews, &pSRV);
	};
	SetSRV(iblMesh.GetSpecularTextureFileRef().get(), 0, 1);
	SetSRV(iblMesh.GetDiffuseTextureFileRef().get(), 1, 1);
	SetSRV(iblMesh.GetBRDFTextureFileRef().get(), 2, 1);

	SetSRV(pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetTextureImageFileRef(AO_TEXUTRE_MAP).get(), 3, 1);
	SetSRV(pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetTextureImageFileRef(COLOR_TEXTURE_MAP).get(), 4, 1);
	SetSRV(pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetTextureImageFileRef(DIFFUSE_TEXTURE_MAP).get(), 5, 1);
	SetSRV(pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetTextureImageFileRef(SPECULAR_TEXTURE_MAP).get(), 6, 1);
	SetSRV(pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetTextureImageFileRef(METALNESS_TEXTURE_MAP).get(), 7, 1);
	SetSRV(pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetTextureImageFileRef(ROUGHNESS_TEXTURE_MAP).get(), 8, 1);
	SetSRV(pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetTextureImageFileRef(EMISSION_TEXTURE_MAP).get(), 9, 1);
	SetSRV(pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetTextureImageFileRef(NORMAL_TEXTURE_MAP).get(), 10, 1);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, idObject.GetObjectBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, viewable.GetPositionBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, pbrStaticMesh.GetPBRConstantBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, pbrStaticMesh.GetMeshFileRef()->GetMaterial()->GetPBRTextureFlagBuffer());

	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, DirectXDevice::ppClampSampler);
}

void PBRIBLLightPixelShader::ResetShader()
{
	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(6, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(7, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(8, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, &pNullSampler);
}
