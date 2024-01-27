#pragma once

#include "ViewableRenderTarget.h"
#include "ViewableDepthStencil.h"
#include "SwapChainAccessable.h"
#include "AFilter.h"

class ACamera
	: public ViewableRenderTarget,
	public ViewableDepthStencil, 
	public AFilter,
	public SwapChainAccessable
{
public:
	ACamera(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchRadIn,
		const float& fYawRadIn,
		const float& fRollRadIn,
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
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override final;
	virtual void SetUAVBarrier() override final;
};

