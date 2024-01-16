#include "ITexture2D.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ITexture2D::ITexture2D()
	: IRectangle(), eFormat(DXGI_FORMAT_UNKNOWN)
{
}

ITexture2D::ITexture2D(
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiArraySizeIn, const UINT& uiNumQualityLevelsIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	const D3D11_USAGE& eUsageIn,
	const DXGI_FORMAT& eFormatIn
)
	: IRectangle(uiWidthIn, uiHeightIn, uiArraySizeIn, uiNumQualityLevelsIn), eFormat(eFormatIn)
{
	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice,
		uiWidth, uiHeight,
		uiArraySize,
		uiNumQualityLevels,
		uiBindFlagIn,
		uiCPUAccessIn,
		uiMiscFlagIn,
		eUsageIn,
		eFormat, 
		cpTexture2D.GetAddressOf()
	);
}

ITexture2D::ITexture2D(
	const UINT& uiWidthIn, 
	const UINT& uiHeightIn,
	const UINT& uiBindFlagIn, 
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn, 
	const D3D11_USAGE& eUsageIn,
	const DXGI_FORMAT& eFormatIn,
	uint8_t* pImageSourceIn
)
	: IRectangle(uiWidthIn, uiHeightIn, 1, 0), eFormat(eFormatIn)
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

ITexture2D::~ITexture2D()
{
}
