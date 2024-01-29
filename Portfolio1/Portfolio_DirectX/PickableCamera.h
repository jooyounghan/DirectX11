#pragma once
#include "FilteredCamera.h"
#include "IDPickableRenderTarget.h"

class PickableCamera : public FilteredCamera, public IDPickableRenderTarget
{
public:
	PickableCamera(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const float& fFovRadIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn,
		DXGI_FORMAT eDSVFormatIn
	);
	virtual ~PickableCamera();

public:
	virtual void ClearRTV() override;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;

public:
	virtual uint32_t GetPickedID() override;
};

