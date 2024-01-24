#pragma once

#include "AViewable.h"
#include "ARenderTarget.h"
#include "ADepthStencil.h"
#include "ASwapChainAccessable.h"
#include "IFilter.h"

class ACamera
	: public ARenderTarget, public ADepthStencil, 
	public IFilter, public ASwapChainAccessable, public AViewable
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
	virtual void Resolve() = 0;

public:
	virtual void SetAsSwapChainBackBuffer() override;
	virtual void ReleaseAndGetAddressOfFromSwapChain() override;

public:
	inline bool IsDiffWithBackBuffer() {
		return ASwapChainAccessable::uiNumQualityLevels != ARenderTarget::uiNumQualityLevels ||
		ASwapChainAccessable::eFormat != ARenderTarget::eFormat;
	}

public:
	virtual void Apply(ID3D11ShaderResourceView** ppInputSRV);
	virtual void SetUAVBarrier();

public:
	virtual DirectX::XMMATRIX GetTranformMat() = 0;
};

