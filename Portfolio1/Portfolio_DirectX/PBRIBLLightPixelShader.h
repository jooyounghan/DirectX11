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
	void SetShader(class AIBLMesh& iblMesh, class PBRStaticMesh& pbrStaticMesh, class Viewable& viewable);
	void ResetShader();
};
