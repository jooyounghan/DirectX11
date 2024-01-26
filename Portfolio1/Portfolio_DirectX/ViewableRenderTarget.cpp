#include "ViewableRenderTarget.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ViewableRenderTarget::ViewableRenderTarget(
	const float& fXPos, 
	const float& fYPos, 
	const float& fZPos, 
	const float& fFovRadianIn, 
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiWidthIn, 
	const UINT& uiHeightIn, 
	const UINT& uiNumQualityLevelsIn, 
	DXGI_FORMAT eRTVFormatIn
)
	: RenderTarget(
		uiWidthIn, uiHeightIn, 1, uiNumQualityLevelsIn,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		NULL, NULL, D3D11_USAGE_DEFAULT, eRTVFormatIn
	),
	Viewable(
		fXPos, fYPos, fZPos,
		(float)uiWidthIn, (float)uiHeightIn,
		fFovRadianIn, fNearZIn, fFarZIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
}

ViewableRenderTarget::~ViewableRenderTarget()
{
}

void ViewableRenderTarget::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	RenderTarget::Resize(uiWidthIn, uiHeightIn);
	sViewPort.Width = (float)uiWidthIn;
	sViewPort.Height = (float)uiHeightIn;
}
