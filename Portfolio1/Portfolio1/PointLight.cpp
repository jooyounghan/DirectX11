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

PointLight::PointLight(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	: LightInterface(pDeviceIn, pDeviceContextIn)
{
	sBaseLightData.uiLightType = ELightType::PointLight;

	AutoZeroMemory(sPointLightViewProjData);
	ID3D11Helper::CreateBuffer(
		pDevice, sPointLightViewProjData,
		D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, 
		NULL, NULL, 
		cpPointLightViewProjDataBuffer.GetAddressOf()
	);

	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		ID3D11Helper::CreateTexture2D(
			pDevice, 1000.f, 1000.f, 
			1, 0, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 
			NULL, NULL, D3D11_USAGE_DEFAULT, 
			DXGI_FORMAT_R32_TYPELESS, 
			cpShadowMapTexture[idx].GetAddressOf()
		);
		ID3D11Helper::CreateDepthStencilView(pDevice, cpShadowMapTexture[idx].Get(), cpShadowMapDSV[idx].GetAddressOf());
		ID3D11Helper::CreateShaderResoureView(pDevice, cpShadowMapTexture[idx].Get(), cpShadowMapSRV[idx].GetAddressOf());
	}
}

PointLight::~PointLight()
{
}

void PointLight::Update()
{
	LightInterface::Update();

	XMMATRIX tempViewProj;

	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		tempViewProj = MathematicalHelper::MakeViewProjMatrix(sBaseLightData.xmvLocation, xmvDirectDefault[idx], xmvUpDefault[idx], 90.f, 1.f, 0.01f, 1000.f);
		sPointLightViewProjData.xmmViewProj[idx] = XMMatrixTranspose(tempViewProj);
		sPointLightViewProjData.xmmViewProjInv[idx] = XMMatrixInverse(nullptr, tempViewProj);
	}

	ID3D11Helper::UpdateBuffer(pDeviceContext, sPointLightViewProjData, D3D11_MAP_WRITE_DISCARD, cpPointLightViewProjDataBuffer.Get());
}

void PointLight::SetConstantBuffers()
{
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, cpBaseLightDataBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_POINT_LIGHT_VIEWPROJ, 1, cpPointLightViewProjDataBuffer.GetAddressOf());
}

void PointLight::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_POINT_LIGHT_VIEWPROJ, 1, &pResetBuffer);
}

void PointLight::SetShaderResources()
{
	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		pDeviceContext->PSGetShaderResources(PSSRVType::PS_SRV_X_DEPTH + idx, 1, cpShadowMapSRV[idx].GetAddressOf());
	}
}

void PointLight::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		pDeviceContext->PSGetShaderResources(PSSRVType::PS_SRV_X_DEPTH + idx, 1, &pResetSRV);
	}
}
