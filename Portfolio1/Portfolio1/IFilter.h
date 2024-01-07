#pragma once
#include "IShaderResource.h"

class IFilter : public IShaderResource
{
public:
	IFilter(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
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
	~IFilter();

protected:
	ID3D11DeviceContext* pDeviceContext;

protected:
	const UINT uiThreadGroupCntX;
	const UINT uiThreadGroupCntY;
	const UINT uiThreadGroupCntZ;

protected:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> cpUAV;

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) = 0;
	virtual void SetUAVBarrier() = 0;
};

