#include "ViewableDepthOnly.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ID3D11RenderTargetView* ViewableDepthOnly::pNullRTV = nullptr;

ViewableDepthOnly::ViewableDepthOnly(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn,
	const float& fFovDegreeIn,
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
		DXGI_FORMAT_R16_TYPELESS, DXGI_FORMAT_R16_UNORM, D3D11_SRV_DIMENSION_TEXTURE2D
	),
	Viewable(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		(float)uiWidthIn, (float)uiHeightIn, 
		fFovDegreeIn, fNearZIn, fFarZIn
	),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(uiWidthIn, uiHeightIn)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC sDesc;
	AutoZeroMemory(sDesc);
	sDesc.Format = DXGI_FORMAT_D16_UNORM;
	sDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	ID3D11Helper::CreateDepthStencilView(DirectXDevice::pDevice, cpTexture2D.Get(), &sDesc, cpDSV.GetAddressOf());
}

ViewableDepthOnly::~ViewableDepthOnly()
{
}

void ViewableDepthOnly::SetDepthOnlyRenderTarget()
{
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, cpDSV.Get());
	DirectXDevice::pDeviceContext->RSSetViewports(1, &sViewPort);
	ClearDSV();
}

void ViewableDepthOnly::ResetDepthOnlyRenderTarget()
{
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &nullViewPort);
}

void ViewableDepthOnly::ClearDSV()
{
	// TODO : D3D11_CLEAR_STENCIL DELETE
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
		DirectXDevice::pDevice, 
		cpTexture2D.Get(), 
		nullptr, 
		cpDSV.GetAddressOf()
	);

	sViewPort.Width = (float)uiWidthIn;
	sViewPort.Height = (float)uiHeightIn;
}
