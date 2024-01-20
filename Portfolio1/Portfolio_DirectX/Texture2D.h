#pragma once
#include <stdint.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "ARectangle.h"

class Texture2D : virtual public ARectangle
{
public:
	Texture2D();
	Texture2D(
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

	Texture2D(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		const D3D11_USAGE& eUsageIn,
		const DXGI_FORMAT& eFormatIn,
		uint8_t* pImageSourceIn
	);

	virtual ~Texture2D();

public:
	UINT		uiArraySize;
	UINT		uiNumQualityLevels;
	UINT		uiBindFlag;
	UINT		uiCPUAccess;
	UINT		uiMiscFlag;
	D3D11_USAGE eUsage;
	DXGI_FORMAT eFormat;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpTexture2D;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

