#include "PointLight.h"

PointLight::PointLight(
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
) : ALight(fXPos, fYPos, fZPos, uiWidthIn, uiHeightIn)
{
}

PointLight::~PointLight() {}

void PointLight::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
}
