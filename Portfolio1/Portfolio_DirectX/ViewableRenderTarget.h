#pragma once
#include "RenderTarget.h"
#include "Viewable.h"

class ViewableRenderTarget : public RenderTarget, virtual public Viewable
{
public:
	ViewableRenderTarget(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchRadIn,
		const float& fYawRadIn,
		const float& fRollRadIn,
		const float& fFovRadianIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn
	);
	virtual ~ViewableRenderTarget();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
};


