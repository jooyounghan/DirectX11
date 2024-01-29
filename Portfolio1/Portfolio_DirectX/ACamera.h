#pragma once

#include "ViewableRenderTarget.h"
#include "ViewableDepthStencil.h"
#include "SwapChainAccessable.h"
#include "AFilter.h"
#include "APossessable.h"

class ACamera
	: public ViewableRenderTarget,
	public ViewableDepthStencil, 
	public AFilter,
	public SwapChainAccessable,
	public APossessable
{
public:
	ACamera(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn,
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
	virtual void ManageKeyBoardInput(const float& fDelay);
	virtual void ManageMouseInput(const int& iMouseXIn, const int& iMouseYIn);

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	virtual void SetAsBackBufferAddress() override;

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV) override final;
	virtual void SetUAVBarrier() override final;
};

