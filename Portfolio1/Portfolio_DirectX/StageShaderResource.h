#pragma once
#include "IShaderResource.h"

class StageShaderResource : public IShaderResource
{
public:
	StageShaderResource(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiArraySizeIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eFormatIn
	);
	virtual ~StageShaderResource();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
};

