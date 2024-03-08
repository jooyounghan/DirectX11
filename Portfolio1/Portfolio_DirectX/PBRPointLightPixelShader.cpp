#include "PBRPointLightPixelShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "IMesh.h"
#include "PBRStaticMesh.h"
#include "Viewable.h"
#include "PointLight.h"

PBRPointLightPixelShader::PBRPointLightPixelShader()
	: IPixelShader()
{
	ID3D11Helper::CreatePS(
		DirectXDevice::pDevice,
		L"PBRWithPointPS.hlsl",
		cpPixelShader.GetAddressOf()
	);
}

PBRPointLightPixelShader::~PBRPointLightPixelShader()
{
}

void PBRPointLightPixelShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(cpPixelShader.Get(), NULL, NULL);
	DirectXDevice::pDeviceContext->OMSetBlendState(DirectXDevice::pAddingBlendState, NULL, UINT(0xFFFFFFFF));
	DirectXDevice::pDeviceContext->OMSetDepthStencilState(DirectXDevice::pDrawLessEqualDSS, 0);
}

void PBRPointLightPixelShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->OMSetBlendState(nullptr, NULL, UINT(0xFFFFFFFF));
	DirectXDevice::pDeviceContext->OMSetDepthStencilState(nullptr, 0);
}

void PBRPointLightPixelShader::SetShader(PointLight& pointLight, IMesh& idMesh, PBRStaticMesh& pbrStaticMesh, Viewable& viewable)
{
	IImageFile* pImageFile = nullptr;
	ID3D11ShaderResourceView* pSRV = nullptr;
	auto SetSRV = [&](IImageFile* pImage, const UINT& startSlot, const UINT& numViews) {
		pImageFile = pImage;
		pSRV = pImageFile != nullptr ? pImageFile->GetSRV() : nullptr;
		DirectXDevice::pDeviceContext->PSSetShaderResources(startSlot, numViews, &pSRV);
		};

	SetSRV(pbrStaticMesh.GetTextureImageFileRef(COLOR_TEXTURE_MAP).get(), 0, 1);
	SetSRV(pbrStaticMesh.GetTextureImageFileRef(METALNESS_TEXTURE_MAP).get(), 1, 1);
	SetSRV(pbrStaticMesh.GetTextureImageFileRef(ROUGHNESS_TEXTURE_MAP).get(), 2, 1);
	SetSRV(pbrStaticMesh.GetTextureImageFileRef(EMISSION_TEXTURE_MAP).get(), 3, 1);
	SetSRV(pbrStaticMesh.GetTextureImageFileRef(NORMAL_TEXTURE_MAP).get(), 4, 1);
	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, pointLight.GetAddressOfSRV());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, idMesh.GetIDBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, pointLight.GetPositionBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, pointLight.GetBaseLightBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, pbrStaticMesh.GetPBRConstantBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(4, 1, pbrStaticMesh.GetPBRTextureFlagBuffer());
	
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(5, 1, viewable.GetPositionBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(6, 1, pointLight.GetViewable(PointXDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(7, 1, pointLight.GetViewable(PointNegXDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(8, 1, pointLight.GetViewable(PointYDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(9, 1, pointLight.GetViewable(PointNegYDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(10, 1, pointLight.GetViewable(PointZDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(11, 1, pointLight.GetViewable(PointNegZDirection).GetViewProjBuffer());

	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, DirectXDevice::ppClampSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, DirectXDevice::ppCompareBorderSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(3, 1, DirectXDevice::ppCompareClampSampler);
}

void PBRPointLightPixelShader::ResetShader()
{
	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(4, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(5, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(6, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(7, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(8, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(9, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(10, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(11, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, &pNullSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, &pNullSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(3, 1, &pNullSampler);
}
