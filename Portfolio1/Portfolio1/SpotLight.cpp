#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "ShaderTypeEnum.h"

using namespace DirectX;

SpotLight::SpotLight(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn,
	const DirectX::XMVECTOR& xmvLocationIn,
	const DirectX::XMVECTOR& xmvDirectionIn,
	const float* pLightColorIn,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const float& fLightPowerIn,
	const float& fSpotPowerIn
)
	: LightInterface(pDeviceIn, pDeviceContextIn, xmvLocationIn, pLightColorIn, fLightPowerIn)
{
	sBaseLightData.uiLightType = ELightType::SpotLightType;
	sBaseLightData.xmvDirect = xmvDirectionIn;
	sBaseLightData.fFallOffStart = fFallOffStartIn;
	sBaseLightData.fFallOffEnd = fFallOffEndIn;
	sBaseLightData.fSpotPower = fSpotPowerIn;

	AutoZeroMemory(sSpotLightViewProjData);
	ID3D11Helper::CreateBuffer(
		pDevice, sSpotLightViewProjData,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL,
		cpSpotLightViewProjDataBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateTexture2D(
		pDevice, 1000.f, 1000.f,
		1, 0, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_TYPELESS,
		cpShadowMapTexture.GetAddressOf()
	);
	ID3D11Helper::CreateDepthOnlyViews(pDevice, cpShadowMapTexture.Get(), cpShadowMapSRV.GetAddressOf(), cpShadowMapDSV.GetAddressOf());
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
	pDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_CBUFF_LIGHT_VIEWPROJ, 1, cpSpotLightViewProjDataBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, cpBaseLightDataBuffer.GetAddressOf());
}

void SpotLight::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_CBUFF_LIGHT_VIEWPROJ, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, &pResetBuffer);
}

void SpotLight::SetShaderResources()
{
	pDeviceContext->PSGetShaderResources(PSSRVType::PS_SRV_DEPTH_ONLY_OR_X, 1, cpShadowMapSRV.GetAddressOf());
}

void SpotLight::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	pDeviceContext->PSGetShaderResources(PSSRVType::PS_SRV_DEPTH_ONLY_OR_X, 1, &pResetSRV);
}