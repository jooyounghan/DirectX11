#pragma once

#include "IDomainShader.h"

class ModelRendererDomainShader : public IDomainShader
{
public:
	ModelRendererDomainShader();
	virtual ~ModelRendererDomainShader();

private:
	static ID3D11Buffer* const pNullBuffer;
	static ID3D11ShaderResourceView* const pNullSRV;
	static ID3D11SamplerState* const pNullSampler;
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class PBRStaticMesh& pbrStaticMesh, class Viewable* pViewable);
	void ResetShader();

};

