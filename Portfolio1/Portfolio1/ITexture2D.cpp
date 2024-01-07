#include "ITexture2D.h"
#include "ID3D11Helper.h"

ITexture2D::ITexture2D(
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
	: pDevice(pDeviceIn), uiWidth(uiWidthIn), uiHeight(uiHeightIn), uiArraySize(uiArraySizeIn),
	uiNumQualityLevels(uiNumQualityLevelsIn), eFormat(eFormatIn)
{
	ID3D11Helper::CreateTexture2D(
		pDevice,
		uiWidth, uiHeight,
		uiArraySize,
		uiNumQualityLevels,
		uiBindFlagIn | D3D11_BIND_SHADER_RESOURCE, uiCPUAccessIn,
		uiMiscFlagIn, eUsageIn,
		eFormat, cpTexture2D.GetAddressOf()
	);
}

ITexture2D::~ITexture2D()
{
}
