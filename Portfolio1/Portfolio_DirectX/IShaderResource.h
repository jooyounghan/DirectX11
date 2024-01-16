#pragma once
#include "ITexture2D.h"

class IShaderResource : public ITexture2D
{
public:
	IShaderResource();
	IShaderResource(
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
	IShaderResource(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		const D3D11_USAGE& eUsageIn,
		const DXGI_FORMAT& eFormatIn,
		uint8_t* pImageSourceIn
	);
	virtual ~IShaderResource();

public:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpSRV;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

