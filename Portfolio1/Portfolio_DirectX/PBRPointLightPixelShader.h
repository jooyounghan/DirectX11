#pragma once

#include "IPixelShader.h"

class PBRPointLightPixelShader : public IPixelShader
{
public:
	PBRPointLightPixelShader();
	virtual ~PBRPointLightPixelShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class PointLight& pointLight, class IMesh& idMesh, class PBRStaticMesh& pbrStaticMesh, class Viewable& viewable);
	void ResetShader();
};
