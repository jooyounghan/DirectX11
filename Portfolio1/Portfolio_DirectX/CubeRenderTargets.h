#pragma once
#include "ShaderResource.h"
#include "ViewableDepthOnly.h"

enum EDirections : size_t
{
	XDirection,
	NegXDirection,
	YDirection,
	NegYDirection,
	ZDirection,
	NegZDirection,
	DirectionNum
};

class CubeDepthStencilView : public ShaderResource
{
public:
	CubeDepthStencilView(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fFallOffStartIn,
		const float& fFallOffEndIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiNumQualityLevelsIn
	);
	virtual ~CubeDepthStencilView();

protected:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> cpUAVs[DirectionNum];

protected:
	ViewableDepthOnly cubeViewParts[DirectionNum];

public:
	inline Viewable& GetViewable(const EDirections& eDirection) { return cubeViewParts[eDirection]; }
};

