#pragma once
#include "ShaderResource.h"

class IFilter : public ShaderResource
{
public:
	IFilter(
		UINT uiThreadGroupCntXIn,
		UINT uiThreadGroupCntYIn,
		UINT uiThreadGroupCntZIn
	);
	IFilter(
		UINT uiWidthIn,
		UINT uiHeightIn,
		UINT uiArraySizeIn,
		UINT uiNumQualityLevelsIn,
		UINT uiBindFlagIn,
		UINT uiCPUAccessIn,
		UINT uiMiscFlagIn,
		D3D11_USAGE eUsageIn,
		DXGI_FORMAT eFormatIn,
		UINT uiThreadGroupCntXIn,
		UINT uiThreadGroupCntYIn,
		UINT uiThreadGroupCntZIn
	);
	virtual ~IFilter();

protected:
	const UINT uiThreadGroupCntX;
	const UINT uiThreadGroupCntY;
	const UINT uiThreadGroupCntZ;

public:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> cpUAV;

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) = 0;
	virtual void SetUAVBarrier() = 0;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

