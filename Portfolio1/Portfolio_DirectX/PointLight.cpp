#include "PointLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "LightRenderer.h"
#include "ModelRenderer.h"
#include "LightManipulator.h"

size_t PointLight::ullPointLightCnt = 0;
ID3D11RenderTargetView* PointLight::pNullRTV = nullptr;

PointLight::PointLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fLightRColorIn,
	const float& fLightGColorIn,
	const float& fLightBColorIn,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const float& fLightPowerIn
) : 
	ILight(
		fXPos, fYPos, fZPos,
		fLightRColorIn,
		fLightGColorIn,
		fLightBColorIn,
		fFallOffStartIn,
		fFallOffEndIn,
		fLightPowerIn
	),
	CubeDepthStencilView(
		gShadowMapWidth, gShadowMapHeight, 0
	),
	viewable{
		Viewable(fXPos, fYPos, fZPos, 0.f, 90.f, 0.f, gShadowMapWidth, gShadowMapHeight, gLightFovDeg, gLightNearZ, fFallOffEndIn),
		Viewable(fXPos, fYPos, fZPos, 0.f, -90.f, 0.f, gShadowMapWidth, gShadowMapHeight, gLightFovDeg, gLightNearZ, fFallOffEndIn),
		Viewable(fXPos, fYPos, fZPos, -90.f, 0.f, 0.f, gShadowMapWidth, gShadowMapHeight, gLightFovDeg, gLightNearZ, fFallOffEndIn),
		Viewable(fXPos, fYPos, fZPos, 90.f, 0.f, 0.f, gShadowMapWidth, gShadowMapHeight, gLightFovDeg, gLightNearZ, fFallOffEndIn),
		Viewable(fXPos, fYPos, fZPos, 0.f, 0.f, 0.f, gShadowMapWidth, gShadowMapHeight, gLightFovDeg, gLightNearZ, fFallOffEndIn),
		Viewable(fXPos, fYPos, fZPos, 0.f, 180.f, 0.f, gShadowMapWidth, gShadowMapHeight, gLightFovDeg, gLightNearZ, fFallOffEndIn)
	},
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(gShadowMapWidth, gShadowMapHeight)
{
	ullPointLightCnt++;
	ullPointLightId = ullPointLightCnt;
}

PointLight::~PointLight() {}

size_t PointLight::GetLightID() { return ullPointLightId; }

void PointLight::UpdateLight()
{
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sBaseLightData, D3D11_MAP_WRITE_DISCARD,
		cpBaseLightBuffer.Get()
	);

	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		viewable[idx].fFarZ = sBaseLightData.fFallOffEnd;
		DirectX::XMVECTOR& pos = viewable[idx].GetPosition();
		memcpy(pos.m128_f32, xmvPosition.m128_f32, sizeof(float) * 4);
		viewable[idx].UpdatePosition();
		viewable[idx].UpdateView();
	}
}

void PointLight::SetDepthOnlyRenderTarget(const size_t& idx)
{
	ID3D11DepthStencilView* pDSV = cpDSVs[idx].Get();
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, pDSV);
	DirectXDevice::pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.f, NULL);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &viewable[idx].sViewPort);
}

void PointLight::ResetDepthOnlyRenderTarget()
{
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &Viewable::GetNullViewPort());
}

void PointLight::AcceptLightRenderer(LightRenderer* pLightRenderer)
{
	pLightRenderer->RenderLightMap(*this);
}

void PointLight::AcceptSettingForDirectLighting(ModelRenderer* pModelRenderer)
{
	pModelRenderer->SetLight(*this);
}

void PointLight::AcceptResetingForDirectLighting(ModelRenderer* pModelRenderer)
{
	pModelRenderer->ResetLight(*this);
}

void PointLight::AcceptLightList(LightManipulator* pLightManipulator)
{
	pLightManipulator->VisitLightList(*this);
}

void PointLight::AcceptLightSetting(LightManipulator* pLightManipulator)
{
	pLightManipulator->VisitLightSetting(*this);
}

