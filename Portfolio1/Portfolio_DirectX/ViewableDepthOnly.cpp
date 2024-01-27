#include "ViewableDepthOnly.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ViewableDepthOnly::ViewableDepthOnly(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fFovRadianIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiWidthIn,
	const UINT& uiHeightIn
)
	: 
	IDepthStencil(uiWidthIn, uiHeightIn),
	ShaderResource(
		uiWidthIn, uiHeightIn, 1, 0,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL,
		NULL, NULL, D3D11_USAGE_DEFAULT, 
		DXGI_FORMAT_D32_FLOAT
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		(float)uiWidthIn, (float)uiHeightIn, 
		fFovRadianIn, fNearZIn, fFarZIn
	),
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(uiWidthIn, uiHeightIn)
{
	ID3D11Helper::CreateDepthStencilView(DirectXDevice::pDevice, cpTexture2D.Get(), cpDSV.GetAddressOf());
}

ViewableDepthOnly::~ViewableDepthOnly()
{
}

void ViewableDepthOnly::ClearDSV()
{
	DirectXDevice::pDeviceContext->ClearDepthStencilView(
		cpDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f,
		NULL
	);
}

void ViewableDepthOnly::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	cpDSV.Reset();
	ShaderResource::Resize(uiWidthIn, uiHeightIn);
	ID3D11Helper::CreateDepthStencilView(
		DirectXDevice::pDevice, cpTexture2D.Get(), cpDSV.GetAddressOf()
	);

	sViewPort.Width = (float)uiWidthIn;
	sViewPort.Height = (float)uiHeightIn;
}
