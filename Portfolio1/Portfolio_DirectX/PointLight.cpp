#include "PointLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "LightRenderer.h"
#include "ModelRenderer.h"

PointLight::PointLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn
) : 
	ILight(fXPos, fYPos, fZPos), 
	IMovable(fXPos, fYPos, fZPos),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	viewablesDirections{
		{
			fXPos, fYPos, fZPos,
			fPitchDegIn, -90.f + fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchDegIn, 90.f + fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			-90.f + fPitchDegIn, fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			90.f + fPitchDegIn, fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchDegIn, fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchDegIn, fYawDegIn + 180.f, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		}
}
{
	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		sBaseLightData.fFallOffStart = gDefaultFallOffStart;
		sBaseLightData.fFallOffEnd = gDefaultFallOffEnd;
	}
}

PointLight::~PointLight() {}

void PointLight::UpdateLight()
{
	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		viewablesDirections[idx].fNearZ = sBaseLightData.fFallOffStart;
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
	pLightRenderer->VisitLight(*this);
}

void PointLight::AcceptSettingForDirectLighting(ModelRenderer* pModelRenderer)
{
	pModelRenderer->SetLight(*this);
}

void PointLight::AcceptResetingForDirectLighting(ModelRenderer* pModelRenderer)
{
	pModelRenderer->ResetLight(*this);
}

