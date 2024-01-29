#include "ViewableDepthStencil.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ViewableDepthStencil::ViewableDepthStencil(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn,
	const float& fFovRadianIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eDSVFormatIn
)
	: IDepthStencil(
		uiWidthIn, uiHeightIn
	),
	Texture2D(
		uiWidthIn, uiHeightIn, 1, uiNumQualityLevelsIn,
		D3D11_BIND_DEPTH_STENCIL,
		NULL, NULL, D3D11_USAGE_DEFAULT, eDSVFormatIn
	),
	Viewable(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		(float)uiWidthIn, (float)uiHeightIn,
		fFovRadianIn, fNearZIn, fFarZIn
	),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(uiWidthIn, uiHeightIn)
{
	ID3D11Helper::CreateDepthStencilView(DirectXDevice::pDevice, cpTexture2D.Get(), cpDSV.GetAddressOf());
}

ViewableDepthStencil::~ViewableDepthStencil()
{
}


void ViewableDepthStencil::ClearDSV()
{
	DirectXDevice::pDeviceContext->ClearDepthStencilView(
		cpDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f,
		NULL
	);
}

void ViewableDepthStencil::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	cpDSV.Reset();
	Texture2D::Resize(uiWidthIn, uiHeightIn);
	ID3D11Helper::CreateDepthStencilView(
		DirectXDevice::pDevice, cpTexture2D.Get(), cpDSV.GetAddressOf()
	);

	sViewPort.Width = (float)uiWidthIn;
	sViewPort.Height = (float)uiHeightIn;
}
