#pragma once
#include "ITexture2D.h"

class IShaderResource : public ITexture2D
{
public:
	IShaderResource(
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
	);
	~IShaderResource();

public:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpSRV;

public:
	virtual void Resize() = 0;
};

