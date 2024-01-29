#pragma once
#include "ACamera.h"
#include <vector>

class FilteredCamera : public ACamera
{
public:
	FilteredCamera(
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
	virtual ~FilteredCamera();

protected:
	std::vector<AFilter*> pFilters;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	virtual void Resolve() override;
};

