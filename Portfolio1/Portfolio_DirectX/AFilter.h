#pragma once
#include "ShaderResource.h"

class AFilter : public ShaderResource
{
public:
	AFilter(
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
	virtual ~AFilter();

protected:
	const UINT uiThreadGroupCntX;
	const UINT uiThreadGroupCntY;
	const UINT uiThreadGroupCntZ;

protected:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> cpUAV;

public:
	inline void ResetFilter() { cpUAV.ReleaseAndGetAddressOf(); cpUAV = nullptr; }

	inline ID3D11ShaderResourceView** GetAddressOfFilterSRV() { return cpSRV.GetAddressOf(); }

	inline ID3D11UnorderedAccessView* GetFilterUAV() { return cpUAV.Get(); }
	inline ID3D11UnorderedAccessView** GetAddressOfFilterUAV() { return cpUAV.GetAddressOf(); }

public:
	virtual void AcceptFilterList(class CameraManipulator* pCameraManipulator) = 0;
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) = 0;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

