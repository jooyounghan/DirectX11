#include "Texture2D.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

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
		uiWidthIn, uiHeightIn,
		uiArraySizeIn,
		uiNumQualityLevelsIn,
		uiBindFlagIn,
		uiCPUAccessIn,
		uiMiscFlagIn,
		eUsageIn,
		eFormatIn,
		cpTexture2D.GetAddressOf()
	);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	uiWidth = uiWidthIn;
	uiHeight = uiHeightIn;

	cpTexture2D.Reset();
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
