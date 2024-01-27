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
		const float& fFovRadianIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);
	virtual ~ViewableDepthOnly();

public:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> cpDSV;

public:
	virtual void ClearDSV();
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};

