#include "PointLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

PointLight::PointLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchRadIn,
	const float& fYawRadIn,
	const float& fRollRadIn
) : 
	ILight(fXPos, fYPos, fZPos), 
	IMovable(fXPos, fYPos, fZPos),
	IAngleAdjustable(fPitchRadIn, fYawRadIn, fRollRadIn),
	viewablesDirections{
		{
			fXPos, fYPos, fZPos,
			fPitchRadIn, DirectX::XMConvertToRadians(-90.f) + fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(90.f),
			sBaseLightData.fFallOffStart,
			sBaseLightData.fFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchRadIn, DirectX::XMConvertToRadians(90.f) + fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(90.f),
			sBaseLightData.fFallOffStart,
			sBaseLightData.fFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			DirectX::XMConvertToRadians(-90.f) + fPitchRadIn, fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(90.f),
			sBaseLightData.fFallOffStart,
			sBaseLightData.fFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			DirectX::XMConvertToRadians(90.f) + fPitchRadIn, fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(90.f),
			sBaseLightData.fFallOffStart,
			sBaseLightData.fFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchRadIn, fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(90.f),
			sBaseLightData.fFallOffStart,
			sBaseLightData.fFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchRadIn, fYawRadIn + DirectX::XMConvertToRadians(180.f), fRollRadIn,
			DirectX::XMConvertToRadians(90.f),
			sBaseLightData.fFallOffStart,
			sBaseLightData.fFallOffEnd,
			1000, 1000
		}
}
{
}

PointLight::~PointLight() {}


void PointLight::UpdateLight()
{
	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		viewablesDirections[idx].fNearZ = sBaseLightData.fFallOffStart;
		viewablesDirections[idx].fFarZ = sBaseLightData.fFallOffEnd;
	}

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sBaseLightData, D3D11_MAP_WRITE_DISCARD,
		cpBaseLightBuffer.Get()
	);
}

void PointLight::UpdateView()
{
	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		viewablesDirections[idx].UpdateView();
	}
}
