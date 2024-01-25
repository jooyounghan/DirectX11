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
	DXGI_FORMAT eRTVFormatIn,
	DXGI_FORMAT eDSVFormatIn
)
	: ARenderTarget(
		uiWidthIn, uiHeight, 1, uiNumQualityLevelsIn, 
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		NULL, NULL, D3D11_USAGE_DEFAULT, eRTVFormatIn
	),
	ADepthStencil(
		uiWidthIn, uiHeightIn, 1, uiNumQualityLevelsIn, 
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL,
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

void ViewableRenderTarget::ClearDSV()
{
	DirectXDevice::pDeviceContext->ClearDepthStencilView(
		cpDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f,
		NULL
	);
}

void ViewableRenderTarget::ClearRTV()
{
	DirectXDevice::pDeviceContext->ClearRenderTargetView(ARenderTarget::cpRTV.Get(), ARenderTarget::fClearColor);
}

void ViewableRenderTarget::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	// Viewable ∞¥√º Resize
	uiWidth = uiWidthIn;
	uiHeight = uiHeightIn;
	sViewPort.Width = (float)uiHeight;
	sViewPort.Height = (float)uiHeight;

	// DepthStencil ∞¥√º Resize
	cpDSV.Reset();
	ADepthStencil::cpSRV.Reset();
	ADepthStencil::cpTexture2D.Reset();

	CreateTexture2DAndSRV<ADepthStencil>();
	ID3D11Helper::CreateDepthStencilView(
		DirectXDevice::pDevice,
		ADepthStencil::cpTexture2D.Get(),
		cpDSV.GetAddressOf()
	);

	// RenderTargetø° ¥Î«— √ ±‚»≠
	cpRTV.Reset();
	ARenderTarget::cpSRV.Reset();
	ARenderTarget::cpTexture2D.Reset();

	CreateTexture2DAndSRV<ARenderTarget>();
	ID3D11Helper::CreateRenderTargetView(
		DirectXDevice::pDevice,
		ARenderTarget::cpTexture2D.Get(),
		cpRTV.GetAddressOf()
	);
}
