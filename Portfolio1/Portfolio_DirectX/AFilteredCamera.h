#pragma once
#include "ACamera.h"
#include <vector>

class AFilteredCamera : public ACamera
{
public:
	AFilteredCamera(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const float& fFovRadIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn,
		DXGI_FORMAT eDSVFormatIn
	);
	virtual ~AFilteredCamera();

protected:
	std::vector<IFilter*> pFilters;

public:
	virtual void ClearRTV() = 0;
	virtual void ClearDSV() = 0;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
	virtual void UpdateCamera(const float& fDelta) = 0;
	virtual void Resolve();
};

