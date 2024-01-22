#include "ADepthStencil.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ADepthStencil::ADepthStencil(
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
	: Texture2D(
		uiWidthIn, uiHeightIn, uiArraySizeIn,
		uiNumQualityLevelsIn, uiBindFlagIn, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn, eFormatIn
	)
{
	ID3D11Helper::CreateDepthStencilView(DirectXDevice::pDevice, cpTexture2D.Get(), cpDSV.GetAddressOf());
}

ADepthStencil::~ADepthStencil()
{
}

void ADepthStencil::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	cpDSV.Reset();
	cpDSV = nullptr;
	Texture2D::Resize(uiWidthIn, uiHeightIn);
	ID3D11Helper::CreateDepthStencilView(DirectXDevice::pDevice, cpTexture2D.Get(), cpDSV.GetAddressOf());
}
