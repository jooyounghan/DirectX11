#pragma once

#include "IPixelShader.h"

class PBRIBLLightPixelShader : public IPixelShader
{
public:
	PBRIBLLightPixelShader();
	virtual ~PBRIBLLightPixelShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader();
	void ResetShader();
};
