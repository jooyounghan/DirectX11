#include "IRenderTarget.h"
#include "ID3D11Helper.h"

IRenderTarget::IRenderTarget(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
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
	: IShaderResource(
		pDeviceIn, pDeviceContextIn, uiWidthIn, uiHeightIn, uiArraySizeIn,
		uiNumQualityLevelsIn, uiBindFlagIn | D3D11_BIND_RENDER_TARGET, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn, eFormatIn
	)
{
	ID3D11Helper::CreateRenderTargetView(pDeviceIn, cpTexture2D.Get(), cpRTV.GetAddressOf());
}

IRenderTarget::~IRenderTarget()
{
}
