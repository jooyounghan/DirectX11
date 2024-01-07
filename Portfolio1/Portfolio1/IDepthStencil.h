#pragma once
#include "ITexture2D.h"

class IDepthStencil : public ITexture2D
{
public:
	IDepthStencil(
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
	~IDepthStencil();

public:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		cpDSV;

public:
	virtual void ClearDSV() = 0;
	virtual void Resize() = 0;
};

