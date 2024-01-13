#include "ITexture2D.h"
#include "ID3D11Helper.h"

ITexture2D::ITexture2D(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const UINT& uiArraySizeIn, const UINT& uiNumQualityLevelsIn,
	const UINT& uiBindFlagIn,
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn,
	D3D11_USAGE eUsageIn,
	DXGI_FORMAT eFormatIn
)
	: IDirectXDevice(pDeviceIn, pDeviceContextIn), 
	IRectangle(uiWidthIn, uiHeightIn, uiArraySizeIn, uiNumQualityLevelsIn), eFormat(eFormatIn)
{
	ID3D11Helper::CreateTexture2D(
		pDevice,
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
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn, 
	const UINT& uiWidthIn, 
	const UINT& uiHeightIn,
	const UINT& uiBindFlagIn, 
	const UINT& uiCPUAccessIn,
	const UINT& uiMiscFlagIn, 
	D3D11_USAGE eUsageIn,
	DXGI_FORMAT eFormatIn,
	const uint8_t* const pImageSourceIn
)
{
	ID3D11Helper::CreateTexture2D(
		pDevice,
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
