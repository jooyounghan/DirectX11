#pragma once
#include "IPixelShader.h"

class NormalVectorPixelShader : public IPixelShader
{
public:
	NormalVectorPixelShader();
	virtual ~NormalVectorPixelShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader();
	void ResetShader();
};

