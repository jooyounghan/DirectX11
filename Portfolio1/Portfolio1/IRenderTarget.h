#pragma once
#include "IShaderResource.h"

class IRenderTarget : public IShaderResource
{
public:
	IRenderTarget(
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
	~IRenderTarget();

public:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpRTV;

public:
	virtual void ClearRTV() = 0;
	virtual void Resize() = 0;
};

