#pragma once

#include "AViewable.h"
#include "ARenderTarget.h"
#include "ADepthStencil.h"
#include "ASwapChainAccessable.h"

class ACamera : public ARenderTarget, public ADepthStencil, public ASwapChainAccessable, public AViewable
{
public:
	ACamera(
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
	virtual ~ACamera();

public:
	virtual void ClearRTV() = 0;
	virtual void ClearDSV() = 0;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
	virtual void UpdateCamera(const float& fDelta) = 0;
	virtual void Resolve() = 0;

public:
	virtual DirectX::XMMATRIX GetTranformMat() = 0;
};

