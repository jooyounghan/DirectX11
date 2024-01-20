#pragma once
#include "ShaderResource.h"

class StageShaderResource : public ShaderResource
{
public:
	StageShaderResource(
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiArraySizeIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eFormatIn
	);
	virtual ~StageShaderResource();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
};

