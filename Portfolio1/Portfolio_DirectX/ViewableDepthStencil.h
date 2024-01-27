#pragma once
#include "IDepthStencil.h"
#include "Texture2D.h"
#include "Viewable.h"

class ViewableDepthStencil : public IDepthStencil, public Texture2D, virtual public Viewable
{
public:
	ViewableDepthStencil(
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
		DXGI_FORMAT eDSVFormatIn
	);
	virtual ~ViewableDepthStencil();

public:
	virtual void ClearDSV() override;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
};

