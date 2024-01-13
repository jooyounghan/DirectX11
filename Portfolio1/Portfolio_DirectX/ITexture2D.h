#pragma once
#include "IDirectXDevice.h"
#include "IRectangle.h"

class ITexture2D : virtual public IDirectXDevice, virtual public IRectangle
{
public:
	ITexture2D(
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

	ITexture2D(
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

	virtual ~ITexture2D();

protected:
	DXGI_FORMAT eFormat;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpTexture2D;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
};

