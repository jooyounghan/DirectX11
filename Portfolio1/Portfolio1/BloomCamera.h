#pragma once
#include "NormalCamera.h"

class BloomCamera : public NormalCamera
{
public:
	BloomCamera(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQuality, const DXGI_FORMAT& eCameraFormatIn,
		const float& fPosXIn, const float& fPosYIn, const float& fPosZIn,
		const float& fFovDegreeIn, const float& fNearZIn, const float& fFarZIn
	);
	virtual ~BloomCamera() override;
};

