#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "ShaderTypeEnum.h"

using namespace DirectX;

SpotLight::SpotLight(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	: LightInterface(pDeviceIn, pDeviceContextIn)
{
	AutoZeroMemory(sSpotLightViewProjData);
	ID3D11Helper::CreateBuffer(
		pDevice, sSpotLightViewProjData,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		NULL, NULL,
		cpSpotLightViewProjDataBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateTexture2D(
		pDevice, 1000.f, 1000.f,
		1, 0, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_TYPELESS,
		cpShadowMapTexture.GetAddressOf()
	);
	ID3D11Helper::CreateDepthStencilView(pDevice, cpShadowMapTexture.Get(), cpShadowMapDSV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(pDevice, cpShadowMapTexture.Get(), cpShadowMapSRV.GetAddressOf());
}

SpotLight::~SpotLight()
{
}

void SpotLight::Update()
{
	LightInterface::Update();

	const XMVECTOR& xmvDirect = sBaseLightData.xmvDirect;
	XMVECTOR xmvUpDirect = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	xmvUpDirect = xmvUpDirect - XMVector3Dot(xmvDirect, xmvUpDirect) / XMVector3Dot(xmvDirect, xmvDirect) * xmvDirect;

	XMMATRIX tempViewProj = MathematicalHelper::MakeViewProjMatrix(sBaseLightData.xmvLocation, sBaseLightData.xmvDirect, xmvUpDirect, 90.f, 1.f, 0.01f, 1000.f);
	sSpotLightViewProjData.xmmViewProj = XMMatrixTranspose(tempViewProj);
	sSpotLightViewProjData.xmmViewProjInv = XMMatrixInverse(nullptr, tempViewProj);
	ID3D11Helper::UpdateBuffer(pDeviceContext, sSpotLightViewProjData, D3D11_MAP_WRITE_DISCARD, cpSpotLightViewProjDataBuffer.Get());
}

void SpotLight::SetConstantBuffers()
{
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, cpBaseLightDataBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_SPOT_LIGH_VIEW_PROJ, 1, cpSpotLightViewProjDataBuffer.GetAddressOf());
}

void SpotLight::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_SPOT_LIGH_VIEW_PROJ, 1, &pResetBuffer);
}

void SpotLight::SetShaderResources()
{
	pDeviceContext->PSGetShaderResources(PSSRVType::PS_SRV_X_DEPTH, 1, cpShadowMapSRV.GetAddressOf());
}

void SpotLight::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	pDeviceContext->PSGetShaderResources(PSSRVType::PS_SRV_X_DEPTH, 1, &pResetSRV);
}