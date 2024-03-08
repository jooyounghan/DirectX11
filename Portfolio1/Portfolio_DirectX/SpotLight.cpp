#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "LightRenderer.h"
#include "ModelRenderer.h"
#include "LightManipulator.h"

size_t SpotLight::ullSpotLightCnt = 0;

SpotLight::SpotLight(
	const float& fXPosIn,
	const float& fYPosIn,
	const float& fZPosIn,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn,
	const float& fLightRColorIn,
	const float& fLightGColorIn,
	const float& fLightBColorIn,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const float& fLightPowerIn,
	const float& fSpotPowerIn
)
	: ILight(
		fXPosIn, fYPosIn, fZPosIn,
		fLightRColorIn, fLightGColorIn, fLightBColorIn,
		fFallOffStartIn, fFallOffEndIn, fLightPowerIn
		),
	ViewableDepthOnly(
		fXPosIn, fYPosIn, fZPosIn,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		gLightFovDeg,
		gLightNearZ, fFallOffEndIn,
		gShadowMapWidth, gShadowMapHeight
	),
	Viewable(
		fXPosIn, fYPosIn, fZPosIn,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		(float)gShadowMapWidth,
		(float)gShadowMapHeight,
		90.f,
		gLightNearZ, fFallOffEndIn
	),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	IMovable(fXPosIn, fYPosIn, fZPosIn),
	IRectangle(gShadowMapWidth, gShadowMapHeight)
{
	ullSpotLightCnt++;
	ullSpotLightId = ullSpotLightCnt;

	sSpotLightData.fSpotPower = fSpotPowerIn;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice,
		sSpotLightData, D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE,
		NULL, cpSpotLightBuffer.GetAddressOf()
	);
}

SpotLight::~SpotLight()
{

}

size_t SpotLight::GetLightID() { return ullSpotLightId; }

void SpotLight::UpdateLight()
{
	fFarZ = sBaseLightData.fFallOffEnd;

	ViewableDepthOnly::UpdateView();

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sBaseLightData, D3D11_MAP_WRITE_DISCARD,
		cpBaseLightBuffer.Get()
	);

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sSpotLightData, D3D11_MAP_WRITE_DISCARD,
		cpSpotLightBuffer.Get()
	);

}

void SpotLight::AcceptLightRenderer(LightRenderer* pLightRenderer)
{
	pLightRenderer->RenderLightMap(*this);
}

void SpotLight::AcceptPBRDirectLighting(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderWithLight(*this);
}

void SpotLight::AcceptLightList(LightManipulator* pLightManipulator)
{
	pLightManipulator->VisitLightList(*this);
}

void SpotLight::AcceptLightSetting(LightManipulator* pLightManipulator)
{
	pLightManipulator->VisitLightSetting(*this);
}
