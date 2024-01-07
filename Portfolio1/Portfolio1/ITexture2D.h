#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

class ITexture2D
{
public:
	ITexture2D(
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
	~ITexture2D();

protected:
	ID3D11Device* pDevice;

protected:
	UINT uiWidth;
	UINT uiHeight;
	UINT uiArraySize;
	UINT uiNumQualityLevels;
	DXGI_FORMAT eFormat;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpTexture2D;

public:
	virtual void Resize() = 0;
};

