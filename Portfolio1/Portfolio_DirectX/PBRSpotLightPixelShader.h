#pragma once

#include "IPixelShader.h"

class PBRSpotLightPixelShader : public IPixelShader
{
public:
	PBRSpotLightPixelShader();
	virtual ~PBRSpotLightPixelShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader();
	void ResetShader();
};

