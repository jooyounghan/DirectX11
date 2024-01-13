#pragma once

#include "AViewable.h"
#include "IRenderTarget.h"
#include "IDepthStencil.h"
#include "ASwapChainAccessable.h"

class ICamera : public IRenderTarget, public IDepthStencil, public ASwapChainAccessable, public AViewable
{
public:
	ICamera(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		IDXGISwapChain* pSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const float& fFovRadIn, 
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn,
		DXGI_FORMAT eDSVFormatIn
	);
	virtual ~ICamera();

public:
	virtual void ClearRTV() = 0;
	virtual void ClearDSV() = 0;
	virtual void Resize() = 0;
	virtual void Update(const float& fDelta) = 0;

public:
	virtual DirectX::XMMATRIX GetTranformMat() = 0;
};

