#pragma once
#include <stdint.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "IRectangle.h"

class ITexture2D : virtual public IRectangle
{
public:
	ITexture2D();
	ITexture2D(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiArraySizeIn,
		const UINT& uiNumQualityLevelsIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		const D3D11_USAGE& eUsageIn,
		const DXGI_FORMAT& eFormatIn
	);

	ITexture2D(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		const D3D11_USAGE& eUsageIn,
		const DXGI_FORMAT& eFormatIn,
		uint8_t* pImageSourceIn
	);

	virtual ~ITexture2D();

protected:
	DXGI_FORMAT eFormat;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpTexture2D;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

