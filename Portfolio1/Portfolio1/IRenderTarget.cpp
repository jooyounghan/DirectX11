#include "IRenderTarget.h"
#include "ID3D11Helper.h"

IRenderTarget::IRenderTarget(
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
	: IShaderResource(
		pDeviceIn, uiWidthIn, uiHeightIn, uiArraySizeIn,
		uiNumQualityLevelsIn, uiBindFlagIn, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn, eFormatIn
	)
{
	ID3D11Helper::CreateRenderTargetView(pDeviceIn, cpTexture2D.Get(), cpRTV.GetAddressOf());
}

IRenderTarget::~IRenderTarget()
{
}
