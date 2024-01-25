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
	: ShaderResource(
		uiWidthIn, uiHeightIn, uiArraySizeIn,
		uiNumQualityLevelsIn, uiBindFlagIn | D3D11_BIND_SHADER_RESOURCE, 
		uiCPUAccessIn, uiMiscFlagIn, eUsageIn, eFormatIn
	)
{
	ID3D11Helper::CreateDepthStencilView(DirectXDevice::pDevice, cpTexture2D.Get(), cpDSV.GetAddressOf());
}

ADepthStencil::~ADepthStencil()
{
}