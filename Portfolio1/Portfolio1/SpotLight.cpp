#include "SpotLight.h"
#include "SpotLight.h"
#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "ShaderTypeEnum.h"
#include "ViewPort.h"

using namespace DirectX;

SpotLight::SpotLight(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn,
	const DirectX::XMVECTOR& xmvLocationIn,
	const DirectX::XMVECTOR& xmvAnglesIn,
	const DirectX::XMVECTOR& xmvLightColorIn,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const float& fLightPowerIn,
	const float& fSpotPowerIn
)
	: LightInterface(pDeviceIn, pDeviceContextIn)
{
	AutoZeroMemory(sSpotLightSet);
	sSpotLightSet.xmvLocation = xmvLocationIn;
	sSpotLightSet.xmvAngles = xmvAnglesIn;
	sSpotLightSet.xmvLightColor = xmvLightColorIn;
	sSpotLightSet.fFallOffStart = fFallOffStartIn;
	sSpotLightSet.fFallOffEnd = fFallOffEndIn;
	sSpotLightSet.fLightPower = fLightPowerIn;
	sSpotLightSet.fSpotPower = fSpotPowerIn;

	ID3D11Helper::CreateBuffer(
		pDevice, sSpotLightSet,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL,
		cpBaseLightDataBuffer.GetAddressOf()
	);

	AutoZeroMemory(sSpotLightViewProjData);
	ID3D11Helper::CreateBuffer(
		pDevice, sSpotLightViewProjData,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL,
		cpSpotLightViewProjDataBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateTexture2D(
		pDevice, 1000, 1000,
		1, 0, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_D32_FLOAT,
		cpShadowMapTexture.GetAddressOf()
	);
	ID3D11Helper::CreateDepthOnlyViews(pDevice, cpShadowMapTexture.Get(), cpShadowMapSRV.GetAddressOf(), cpShadowMapDSV.GetAddressOf());
}

SpotLight::~SpotLight()
{
}


void SpotLight::Update()
{

	const XMMATRIX& xmmAffineMat = MathematicalHelper::MakeAffineTransformation(
		1.f, 1.f, 1.f,
		XMConvertToRadians(sSpotLightSet.xmvAngles.m128_f32[0]),
		XMConvertToRadians(sSpotLightSet.xmvAngles.m128_f32[1]),
		XMConvertToRadians(sSpotLightSet.xmvAngles.m128_f32[2]),
		0.f, 0.f, 0.f
	);

	const XMVECTOR& xmvDirect = XMVector4Transform(
		XMVectorSet(0.f, 0.f, 1.f, 0.f),
		xmmAffineMat
	);
	XMVECTOR xmvUpDirect = XMVector4Transform(
		XMVectorSet(0.f, 1.f, 0.f, 0.f),
		xmmAffineMat
	);


	// 업데이트할 때에는 계산된 방향 벡터를 각도 항에 넣어주어 Shader에서 추가 계산을 수행하지 않는다.
	XMVECTOR xmvAngles = sSpotLightSet.xmvAngles;
	sSpotLightSet.xmvAngles = xmvDirect;
	ID3D11Helper::UpdateBuffer(pDeviceContext, sSpotLightSet, D3D11_MAP_WRITE_DISCARD, cpBaseLightDataBuffer.Get());
	sSpotLightSet.xmvAngles = xmvAngles;

	XMMATRIX tempViewProj = MathematicalHelper::MakeViewProjMatrix(
		sSpotLightSet.xmvLocation, 
		xmvDirect, xmvUpDirect, 
		XMConvertToRadians(90.f), 
		1.f, 
		0.01f, 
		1000.f
	);

	sSpotLightViewProjData.xmmViewProj = XMMatrixTranspose(tempViewProj);
	sSpotLightViewProjData.xmmViewProjInv = XMMatrixInverse(nullptr, tempViewProj);
	ID3D11Helper::UpdateBuffer(pDeviceContext, sSpotLightViewProjData, D3D11_MAP_WRITE_DISCARD, cpSpotLightViewProjDataBuffer.Get());
}

void SpotLight::SetConstantBuffers()
{
	pDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_CBUFF_LIGHT_VIEWPROJ, 1, cpSpotLightViewProjDataBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHT_VIEWPROJ, 1, cpSpotLightViewProjDataBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, cpBaseLightDataBuffer.GetAddressOf());
}

void SpotLight::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_CBUFF_LIGHT_VIEWPROJ, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHT_VIEWPROJ, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, &pResetBuffer);
}

void SpotLight::SetShaderResources()
{
	pDeviceContext->PSSetShaderResources(PSSRVType::PS_SRV_DEPTH_ONLY_OR_X, 1, cpShadowMapSRV.GetAddressOf());
}

void SpotLight::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	pDeviceContext->PSSetShaderResources(PSSRVType::PS_SRV_DEPTH_ONLY_OR_X, 1, &pResetSRV);
}

void SpotLight::OMSetRenderTarget()
{
	ID3D11RenderTargetView* nullRTV = nullptr;
	pDeviceContext->OMSetRenderTargets(1, &nullRTV, cpShadowMapDSV.Get());

	ViewPort& viewPort = ViewPort::GetInstance(pDeviceContext);
	viewPort.SetViewPort(0.f, 0.f, 1000.f, 1000.f, 0.f, 1.f);
}

void SpotLight::WipeOut()
{
	pDeviceContext->ClearDepthStencilView(cpShadowMapDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
