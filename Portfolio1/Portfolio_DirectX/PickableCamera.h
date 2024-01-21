#pragma once
#include "AFilteredCamera.h"
#include "IDPickableRenderTarget.h"

class PickableCamera : public AFilteredCamera, public IDPickableRenderTarget
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
	virtual void ClearRTV() override;
	virtual void ClearDSV() override;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	virtual void UpdateCamera(const float& fDelta) override;

public:
	virtual uint32_t GetPickedID() override;

public:
	virtual DirectX::XMMATRIX GetTranformMat();
};

