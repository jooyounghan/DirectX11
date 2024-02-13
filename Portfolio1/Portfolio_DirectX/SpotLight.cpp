#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "LightRenderer.h"
#include "ModelRenderer.h"

SpotLight::SpotLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn
)
	: ILight(fXPos, fYPos, fZPos),
	ViewableDepthOnly(
		fXPos, fYPos, fZPos, 
		fPitchDegIn, fYawDegIn, fRollDegIn,
		DirectX::XMConvertToRadians(gLightFovDeg),
		gDefaultFallOffStart, gDefaultFallOffEnd,
		1000, 1000
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		fPitchDegIn, fYawDegIn, fRollDegIn,
		1000.f, 
		1000.f, 
		DirectX::XMConvertToRadians(90.f),
		gDefaultFallOffStart, gDefaultFallOffEnd
	),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(1000, 1000)
{
	sBaseLightData.fFallOffStart = gDefaultFallOffStart;
	sBaseLightData.fFallOffEnd = gDefaultFallOffEnd;

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

void SpotLight::UpdateLight()
{
	fNearZ = sBaseLightData.fFallOffStart;
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
	pLightRenderer->VisitLight(*this);
}

void SpotLight::AcceptSettingForDirectLighting(ModelRenderer* pModelRenderer)
{
	pModelRenderer->SetLight(*this);
}

void SpotLight::AcceptResetingForDirectLighting(ModelRenderer* pModelRenderer)
{
	pModelRenderer->ResetLight(*this);
}