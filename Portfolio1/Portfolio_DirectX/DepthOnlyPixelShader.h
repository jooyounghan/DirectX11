#pragma once

#include "IPixelShader.h"

class DepthOnlyPixelShader : public IPixelShader
{
public:
	DepthOnlyPixelShader();
	virtual ~DepthOnlyPixelShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader();
	void ResetShader();
};

