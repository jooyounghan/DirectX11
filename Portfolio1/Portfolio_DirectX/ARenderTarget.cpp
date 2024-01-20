#include "ARenderTarget.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ARenderTarget::ARenderTarget(
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
	)
{
	ID3D11Helper::CreateRenderTargetView(DirectXDevice::pDevice, cpTexture2D.Get(), cpRTV.GetAddressOf());
}

ARenderTarget::~ARenderTarget()
{
}

void ARenderTarget::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	ShaderResource::Resize(uiWidthIn, uiHeightIn);
	ID3D11Helper::CreateRenderTargetView(DirectXDevice::pDevice, cpTexture2D.Get(), cpRTV.GetAddressOf());
}
