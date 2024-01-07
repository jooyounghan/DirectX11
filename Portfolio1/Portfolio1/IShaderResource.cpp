#include "IShaderResource.h"
#include "ID3D11Helper.h"

IShaderResource::IShaderResource(
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
	ID3D11Helper::CreateShaderResoureView(pDevice, cpTexture2D.Get(), cpSRV.GetAddressOf());
}

IShaderResource::~IShaderResource()
{
}
