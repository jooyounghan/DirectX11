#pragma once

#include "ViewableRenderTarget.h"
#include "ASwapChainAccessable.h"
#include "IFilter.h"

class ACamera
	: public ViewableRenderTarget, public IFilter, public ASwapChainAccessable
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
	virtual void Resolve() = 0;

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	virtual void SetAsBackBufferAddress() override;

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override;
	virtual void SetUAVBarrier();
};

