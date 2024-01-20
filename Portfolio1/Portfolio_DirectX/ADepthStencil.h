#pragma once
#include "Texture2D.h"

class ADepthStencil : public Texture2D
{
public:
	ADepthStencil(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiArraySizeIn,
		const UINT& uiNumQualityLevelsIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		D3D11_USAGE eUsageIn,
		DXGI_FORMAT eFormatIn
	);
	virtual ~ADepthStencil();

public:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		cpDSV;

public:
	virtual void ClearDSV() = 0;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

