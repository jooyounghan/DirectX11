#pragma once

#include "IDomainShader.h"

class ModelRendererDomainShader : public IDomainShader
{
public:
	ModelRendererDomainShader();
	virtual ~ModelRendererDomainShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class PBRStaticMesh& pbrStaticMesh, class Viewable& viewable);
	void ResetShader();

};

