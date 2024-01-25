#pragma once
#include "ARenderTarget.h"
#include "ADepthStencil.h"
#include "Viewable.h"

class ViewableRenderTarget : public ARenderTarget, public ADepthStencil, public Viewable
{
public:
	ViewableRenderTarget(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fFovRadianIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn,
		DXGI_FORMAT eDSVFormatIn
	);
	~ViewableRenderTarget();

public:
	virtual void ClearRTV();
	virtual void ClearDSV();
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);

protected:
	template <typename T>
	void CreateTexture2DAndSRV();
};

template<typename T>
inline void ViewableRenderTarget::CreateTexture2DAndSRV()
{
	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice, uiWidth, uiHeight,
		T::uiArraySize, T::uiNumQualityLevels,
		T::uiBindFlag, T::uiCPUAccess,
		T::uiMiscFlag, T::eUsage,
		T::eFormat, T::cpTexture2D.GetAddressOf()
	);

	ID3D11Helper::CreateShaderResoureView(
		DirectXDevice::pDevice, T::cpTexture2D.Get(),
		T::cpSRV.GetAddressOf()
	);

}
