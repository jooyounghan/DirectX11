#pragma once
#include "AFilter.h"

class BlurFilter : public AFilter
{
public:
	BlurFilter(
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
	virtual ~BlurFilter();

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override;
	virtual void SetUAVBarrier() override;
};

