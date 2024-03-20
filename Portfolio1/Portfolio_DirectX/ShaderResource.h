#pragma once
#include "Texture2D.h"


class ShaderResource : public Texture2D
{
public:
	ShaderResource(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiArraySizeIn,
		const UINT& uiNumQualityLevelsIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		const D3D11_USAGE& eUsageIn,
		const DXGI_FORMAT& eTextureFormatIn,
		const DXGI_FORMAT& eSRVFormatIn,
		const D3D_SRV_DIMENSION& eDimensionIn
	);

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

	virtual ~ShaderResource();

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpSRV;

public:
	inline void ResetSRV() { cpSRV.ReleaseAndGetAddressOf(); cpSRV = nullptr; }
	inline ID3D11ShaderResourceView* GetSRV() { return cpSRV.Get(); }
	inline ID3D11ShaderResourceView** GetAddressOfSRV() { return cpSRV.GetAddressOf(); }

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};
