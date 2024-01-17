#pragma once

#include "AViewable.h"
#include "IRenderTarget.h"
#include "IDepthStencil.h"
#include "ASwapChainAccessable.h"

class ICamera : public IRenderTarget, public IDepthStencil, public ASwapChainAccessable, public AViewable
{
public:
	ICamera(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
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
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) = 0;
	virtual void Update(const float& fDelta) = 0;
	virtual void Resolve() = 0;

public:
	virtual DirectX::XMMATRIX GetTranformMat() = 0;
};

