#include "IDepthStencil.h"
#include "ID3D11Helper.h"

IDepthStencil::IDepthStencil(
	ID3D11Device* pDeviceIn,
	UINT uiWidthIn,
	UINT uiHeightIn,
	UINT uiArraySizeIn,
	UINT uiNumQualityLevelsIn,
	UINT uiBindFlagIn,
	UINT uiCPUAccessIn,
	UINT uiMiscFlagIn,
	D3D11_USAGE eUsageIn,
	DXGI_FORMAT eFormatIn
)
	: ITexture2D(
		pDeviceIn, uiWidthIn, uiHeightIn, uiArraySizeIn,
		uiNumQualityLevelsIn, uiBindFlagIn, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn, eFormatIn
	)
{
	ID3D11Helper::CreateDepthStencilView(pDeviceIn, cpTexture2D.Get(), cpDSV.GetAddressOf());
}

IDepthStencil::~IDepthStencil()
{
}
