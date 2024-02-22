#include "RenderTarget.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

RenderTarget::RenderTarget(
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const UINT& uiArraySizeIn,
	const UINT& uiNumQualityLevelsIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	D3D11_USAGE eUsageIn,
	DXGI_FORMAT eFormatIn
)
	: ShaderResource(
		uiWidthIn, uiHeightIn, uiArraySizeIn,
		uiNumQualityLevelsIn, uiBindFlagIn | D3D11_BIND_RENDER_TARGET, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn, eFormatIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	ID3D11Helper::CreateRenderTargetView(DirectXDevice::pDevice, cpTexture2D.Get(), nullptr, cpRTV.GetAddressOf());
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::ClearRTV()
{
	DirectXDevice::pDeviceContext->ClearRenderTargetView(cpRTV.Get(), RenderTarget::fClearColor);
}

void RenderTarget::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	cpRTV.Reset();
	ShaderResource::Resize(uiWidthIn, uiHeightIn);
	ID3D11Helper::CreateRenderTargetView(
		DirectXDevice::pDevice,
		cpTexture2D.Get(),
		nullptr, 
		cpRTV.GetAddressOf()
	);
}
