#pragma once
#include "Texture2D.h"


class ShaderResource : public Texture2D
{
public:
	ShaderResource();
	ShaderResource(
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
	ShaderResource(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		const D3D11_USAGE& eUsageIn,
		const DXGI_FORMAT& eFormatIn,
		uint8_t* pImageSourceIn
	);
	virtual ~ShaderResource();

public:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpSRV;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};
