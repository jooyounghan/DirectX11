#pragma once
#include "DepthStencil.h"
#include "Viewable.h"

class ViewableDepthStencil : public DepthStencil, virtual public Viewable
{
public:
	ViewableDepthStencil(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fFovRadianIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eDSVFormatIn
	);
	~ViewableDepthStencil();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
};

