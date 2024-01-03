#include "PointLight.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "ShaderTypeEnum.h"
#include "ViewPort.h"

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
	const DirectX::XMVECTOR& xmvLightColorIn,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const float& fLightPowerIn
)
	: LightInterface(pDeviceIn, pDeviceContextIn)
{
	AutoZeroMemory(sPointLightSet);
	sPointLightSet.xmvLocation = xmvLocationIn;
	sPointLightSet.xmvLightColor = xmvLightColorIn;
	sPointLightSet.fFallOffStart = fFallOffStartIn;
	sPointLightSet.fFallOffEnd = fFallOffEndIn;
	sPointLightSet.fLightPower = fLightPowerIn;

	ID3D11Helper::CreateBuffer(
		pDevice, sPointLightSet,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL,
		cpBaseLightDataBuffer.GetAddressOf()
	);

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
			pDevice, 1000, 1000, 
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
	ID3D11Helper::UpdateBuffer(pDeviceContext, sPointLightSet, D3D11_MAP_WRITE_DISCARD, cpBaseLightDataBuffer.Get());

	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		const XMMATRIX& tempViewProj = MathematicalHelper::MakeViewProjMatrix(
			sPointLightSet.xmvLocation,
			xmvDirectDefault[idx], xmvUpDefault[idx],
			XMConvertToRadians(70.f), 1.f,
			0.01f, 1000.f
		);
		sPointLightViewProjData[idx].xmmViewProj = XMMatrixTranspose(tempViewProj);
		sPointLightViewProjData[idx].xmmViewProjInv = XMMatrixInverse(nullptr, tempViewProj);
		ID3D11Helper::UpdateBuffer(pDeviceContext, sPointLightViewProjData[idx], D3D11_MAP_WRITE_DISCARD, cpPointLightViewProjDataBuffer[idx].Get());
	}
}

void PointLight::SetConstantBuffers() 
{
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, cpBaseLightDataBuffer.GetAddressOf());
}

void PointLight::SetConstantBuffers(const size_t& uiViewProjIdx)
{
	pDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_CBUFF_LIGHT_VIEWPROJ, 1, cpPointLightViewProjDataBuffer[uiViewProjIdx].GetAddressOf());
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
	for (size_t idx = 0; idx < PointViewProjNum; ++idx)
	{
		pDeviceContext->PSSetShaderResources(PSSRVType::PS_SRV_DEPTH_ONLY_OR_X + idx, 1, &pResetSRV);
	}
}

void PointLight::OMSetRenderTarget(const size_t& uiViewProjIdx)
{
	ID3D11RenderTargetView* nullRTV = nullptr;
	pDeviceContext->OMSetRenderTargets(1, &nullRTV, cpShadowMapDSV[uiViewProjIdx].Get());

	ViewPort& viewPort = ViewPort::GetInstance(pDeviceContext);
	viewPort.SetViewPort(0.f, 0.f, 1000.f, 1000.f, 0.f, 1.f);
}

void PointLight::WipeOut(const size_t& uiViewProjIdx)
{
	pDeviceContext->ClearDepthStencilView(cpShadowMapDSV[uiViewProjIdx].Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
