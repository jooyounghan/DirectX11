#include "Texture2D.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

Texture2D::Texture2D()
	: IRectangle(), eFormat(DXGI_FORMAT_UNKNOWN)
{
}

Texture2D::Texture2D(
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiArraySizeIn, const UINT& uiNumQualityLevelsIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	const D3D11_USAGE& eUsageIn,
	const DXGI_FORMAT& eFormatIn
)
	: IRectangle(uiWidthIn, uiHeightIn),
	uiArraySize(uiArraySizeIn), uiNumQualityLevels(uiNumQualityLevelsIn),
	uiBindFlag(uiBindFlagIn), uiCPUAccess(uiCPUAccessIn), uiMiscFlag(uiMiscFlagIn),
	eUsage(eUsageIn), eFormat(eFormatIn)
{
	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice,
		uiWidth, uiHeight,
		uiArraySize,
		uiNumQualityLevels,
		uiBindFlag,
		uiCPUAccess,
		uiMiscFlag,
		eUsage,
		eFormat, 
		cpTexture2D.GetAddressOf()
	);
}

Texture2D::Texture2D(
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn, 
	const D3D11_USAGE& eUsageIn,
	const DXGI_FORMAT& eFormatIn,
	uint8_t* pImageSourceIn
)
	: IRectangle(uiWidthIn, uiHeightIn), eFormat(eFormatIn), uiArraySize(1), uiNumQualityLevels(0)
{
	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice,
		uiWidth, uiHeight,
		uiBindFlagIn,
		uiCPUAccessIn,
		uiMiscFlagIn,
		eUsageIn,
		eFormat,
		pImageSourceIn,
		cpTexture2D.GetAddressOf()
	);
}

Texture2D::~Texture2D()
{
}