#pragma once
#include <stdint.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "IRectangle.h"

class Texture2D : virtual public IRectangle
{
public:
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

protected:
	UINT		uiArraySize;
	UINT		uiNumQualityLevels;
	UINT		uiBindFlag;
	UINT		uiCPUAccess;
	UINT		uiMiscFlag;
	D3D11_USAGE eUsage;
	DXGI_FORMAT eFormat;

public:
	inline const UINT& GetArraySize() { return uiArraySize; }
	inline const UINT& GetQualityLevels() { return uiNumQualityLevels; }
	inline const DXGI_FORMAT& GetFormat() { return eFormat; }

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpTexture2D;

public:
	inline void ResetTexture() { cpTexture2D.ReleaseAndGetAddressOf(); cpTexture2D = nullptr; }
	inline ID3D11Texture2D* GetTexture() { return cpTexture2D.Get(); }
	inline ID3D11Texture2D** GetAddressOfTexture() { return cpTexture2D.GetAddressOf(); }

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

