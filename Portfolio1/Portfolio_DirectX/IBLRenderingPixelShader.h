#pragma once

#include "IPixelShader.h"

class IBLRenderingPixelShader : public IPixelShader
{
public:
	IBLRenderingPixelShader();
	virtual ~IBLRenderingPixelShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class AIBLMesh& iblMesh, class Viewable& viewable);
	void ResetShader();
};

