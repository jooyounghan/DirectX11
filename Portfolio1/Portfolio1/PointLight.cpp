#include "PointLight.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "ShaderTypeEnum.h"

using namespace DirectX;

XMVECTOR PointLight::xmvDirectDefault[6] = 
{
	XMVectorSet(1.f, 0.f, 0.f, 0.f), 
	XMVectorSet(-1.f, 0.f, 0.f, 0.f), 
	XMVectorSet(0.f, 1.f, 0.f, 0.f),
	XMVectorSet(0.f, -1.f, 0.f, 0.f),
	XMVectorSet(0.f, 0.f, 1.f, 0.f),
	XMVectorSet(1.f, 0.f, -1.f, 0.f)
};

XMVECTOR PointLight::xmvUpDefault[6] = 
{
	XMVectorSet(0.f, 1.f, 0.f, 0.f),
	XMVectorSet(0.f, 1.f, 0.f, 0.f),
	XMVectorSet(1.f, 0.f, -1.f, 0.f),
	XMVectorSet(0.f, 0.f, 1.f, 0.f),
	XMVectorSet(0.f, 1.f, 0.f, 0.f),
	XMVectorSet(0.f, 1.f, 0.f, 0.f),
};

PointLight::PointLight(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn,
	const DirectX::XMVECTOR& xmvLocationIn,
	const float* pLightColorIn,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const float& fLightPowerIn
)
	: LightInterface(pDeviceIn, pDeviceContextIn, xmvLocationIn, pLightColorIn, fLightPowerIn)
{
	sBaseLightData.uiLightType = ELightType::PointLightType;
	sBaseLightData.fFallOffStart = fFallOffStartIn;
	sBaseLightData.fFallOffEnd = fFallOffEndIn;

	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		AutoZeroMemory(sPointLightViewProjData[idx]);
		ID3D11Helper::CreateBuffer(
			pDevice, sPointLightViewProjData,
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE, NULL,
			cpPointLightViewProjDataBuffer[idx].GetAddressOf()
		);
	}

	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		ID3D11Helper::CreateTexture2D(
			pDevice, 1000.f, 1000.f, 
			1, 0, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 
			NULL, NULL, D3D11_USAGE_DEFAULT, 
			DXGI_FORMAT_R32_TYPELESS, 
			cpShadowMapTexture[idx].GetAddressOf()
		);
		ID3D11Helper::CreateDepthOnlyViews(pDevice, cpShadowMapTexture[idx].Get(), cpShadowMapSRV[idx].GetAddressOf(), cpShadowMapDSV[idx].GetAddressOf());
	}
}

PointLight::~PointLight()
{
}

void PointLight::Update()
{
	LightInterface::Update();

	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		const XMMATRIX& tempViewProj = MathematicalHelper::MakeViewProjMatrix(sBaseLightData.xmvLocation, xmvDirectDefault[idx], xmvUpDefault[idx], 90.f, 1.f, 0.01f, 1000.f);
		sPointLightViewProjData[idx].xmmViewProj = XMMatrixTranspose(tempViewProj);
		sPointLightViewProjData[idx].xmmViewProjInv = XMMatrixInverse(nullptr, tempViewProj);
		ID3D11Helper::UpdateBuffer(pDeviceContext, sPointLightViewProjData, D3D11_MAP_WRITE_DISCARD, cpPointLightViewProjDataBuffer[idx].Get());
	}
}

void PointLight::SetConstantBuffers() 
{
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, cpBaseLightDataBuffer.GetAddressOf());
}

void PointLight::SetConstantBuffers(const size_t& uiViewProjIdx)
{
	pDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_CBUFF_LIGHT_VIEWPROJ, 1, cpPointLightViewProjDataBuffer[uiViewProjIdx].GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, cpBaseLightDataBuffer.GetAddressOf());
}

void PointLight::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_CBUFF_LIGHT_VIEWPROJ, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, &pResetBuffer);
}

void PointLight::SetShaderResources() 
{
	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		SetShaderResources(idx);
	}
}

void PointLight::SetShaderResources(const size_t& uiViewProjIdx)
{
	pDeviceContext->PSSetShaderResources(PSSRVType::PS_SRV_DEPTH_ONLY_OR_X + uiViewProjIdx, 1, cpShadowMapSRV[uiViewProjIdx].GetAddressOf());
}

void PointLight::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	pDeviceContext->PSGetShaderResources(PSSRVType::PS_SRV_DEPTH_ONLY_OR_X, 1, &pResetSRV);
}
