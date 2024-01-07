#pragma once

#include "IShaderResource.h"
#include "IRenderTarget.h"
#include "IDepthStencil.h"
#include "ASwapChainAccessable.h"

class ICamera : public IRenderTarget, public IDepthStencil, public ASwapChainAccessable
{
public:
	ICamera(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		IDXGISwapChain* pSwapChainIn,
		UINT uiWidthIn, UINT uiHeightIn,
		UINT uiArraySizeIn,
		UINT uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn,
		DXGI_FORMAT eDSVFormatIn
	);
	~ICamera();

public:
	ID3D11DeviceContext* pDeviceContext;

public:
	virtual void ClearRTV() = 0;
	virtual void ClearDSV() = 0;
	virtual void Resize() = 0;
	virtual void UpdateCamera() = 0;
};

