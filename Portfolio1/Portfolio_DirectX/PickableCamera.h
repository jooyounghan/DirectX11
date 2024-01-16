#pragma once
#include "AFilteredCamera.h"

class PickableCamera : public AFilteredCamera
{
public:
	PickableCamera(
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
	virtual ~PickableCamera();

public:
	virtual void ClearRTV();
	virtual void ClearDSV();
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
	virtual void Update(const float& fDelta);

public:
	virtual DirectX::XMMATRIX GetTranformMat();
};

