#include "PointLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "LightRenderer.h"
#include "ModelRenderer.h"
#include "LightManipulator.h"

size_t PointLight::ullPointLightCnt = 0;

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
	IMovable(fXPos, fYPos, fZPos),
	viewablesDirections{
		{
			fXPos, fYPos, fZPos,
			0.f, -90.f, 0.f,
			gLightFovDeg,
			gLightNearZ, fFallOffEndIn,
			gShadowMapWidth, gShadowMapHeight
		},
		{
			fXPos, fYPos, fZPos,
			0.f, 90.f, 0.f,
			gLightFovDeg,
			gLightNearZ, fFallOffEndIn,
			gShadowMapWidth, gShadowMapHeight
		},
		{
			fXPos, fYPos, fZPos,
			-90.f, 0.f, 0.f,
			gLightFovDeg,
			gLightNearZ, fFallOffEndIn,
			gShadowMapWidth, gShadowMapHeight
		},
		{
			fXPos, fYPos, fZPos,
			90.f, 0.f, 0.f,
			gLightFovDeg,
			gLightNearZ, fFallOffEndIn,
			gShadowMapWidth, gShadowMapHeight
		},
		{
			fXPos, fYPos, fZPos,
			0.f, 0.f, 0.f,
			gLightFovDeg,
			gLightNearZ, fFallOffEndIn,
			gShadowMapWidth, gShadowMapHeight
		},
		{
			fXPos, fYPos, fZPos,
			0.f, 180.f, 0.f,
			gLightFovDeg,
			gLightNearZ, fFallOffEndIn,
			gShadowMapWidth, gShadowMapHeight
		}
}
{
	ullPointLightCnt++;
	ullPointLightId = ullPointLightCnt;
}

PointLight::~PointLight() {}

size_t PointLight::GetLightID() { return ullPointLightId; }

void PointLight::UpdateLight()
{
	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		viewablesDirections[idx].fFarZ = sBaseLightData.fFallOffEnd;
		viewablesDirections[idx].UpdateView();
	}

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sBaseLightData, D3D11_MAP_WRITE_DISCARD,
		cpBaseLightBuffer.Get()
	);
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

