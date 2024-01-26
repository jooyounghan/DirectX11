#include "ViewableDepthStencil.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ViewableDepthStencil::ViewableDepthStencil(
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
)
	: DepthStencil(
		uiWidthIn, uiHeightIn, 1, uiNumQualityLevelsIn,
		D3D11_BIND_DEPTH_STENCIL,
		NULL, NULL, D3D11_USAGE_DEFAULT, eDSVFormatIn
	),
	Viewable(
		fXPos, fYPos, fZPos,
		(float)uiWidthIn, (float)uiHeightIn,
		fFovRadianIn, fNearZIn, fFarZIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
}

ViewableDepthStencil::~ViewableDepthStencil()
{
}


void ViewableDepthStencil::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	DepthStencil::Resize(uiWidthIn, uiHeightIn);
	sViewPort.Width = (float)uiWidthIn;
	sViewPort.Height = (float)uiHeightIn;
}
