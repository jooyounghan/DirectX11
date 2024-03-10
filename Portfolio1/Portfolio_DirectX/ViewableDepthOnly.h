#pragma once
#include "ViewableDepthStencil.h"
#include "ShaderResource.h"

class ViewableDepthOnly : public IDepthStencil, public ShaderResource, virtual public Viewable
{
public:
	ViewableDepthOnly(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn,
		const float& fFovDegreeIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);
	virtual ~ViewableDepthOnly();

public:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> cpDSV;

protected:
	static ID3D11RenderTargetView* pNullRTV;

public:
	virtual void SetDepthOnlyRenderTarget();
	virtual void ResetDepthOnlyRenderTarget();

public:
	virtual void ClearDSV();
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

