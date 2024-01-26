#pragma once
#include "ILight.h"
#include "RenderTarget.h"

#include <vector>

class PointLight : public ILight
{
public:
	PointLight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		D3D11_USAGE eUsageIn,
		DXGI_FORMAT eFormatIn
	);
	virtual ~PointLight();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};