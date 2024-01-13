#pragma once
#include "ITexture2D.h"

class IShaderResource : public ITexture2D
{
public:
	IShaderResource(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
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
	IShaderResource(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiBindFlagIn,
		const UINT& uiCPUAccessIn,
		const UINT& uiMiscFlagIn,
		D3D11_USAGE eUsageIn,
		DXGI_FORMAT eFormatIn,
		const uint8_t* const pImageSourceIn

	);
	virtual ~IShaderResource();

public:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpSRV;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

