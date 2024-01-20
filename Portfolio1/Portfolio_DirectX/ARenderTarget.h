#pragma once
#include "ShaderResource.h"

class ARenderTarget : public ShaderResource
{
public:
	ARenderTarget(
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
	virtual ~ARenderTarget();

public:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpRTV;

protected:
	static constexpr float fClearColor[4] = { 0.f, 0.f, 0.f, 0.f };

public:
	virtual void ClearRTV() = 0;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

