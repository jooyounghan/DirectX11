#pragma once

#include "IDomainShader.h"

class PBRStaticMesh;
class Viewable;

struct ModelDSBindingSet
{
	size_t meshIdx;
	PBRStaticMesh* pPbrStaticMesh;
	Viewable* pViewable;
};

class ModelRendererDomainShader : public IDomainShader
{
private:
	ModelRendererDomainShader();
	ModelRendererDomainShader(const ModelRendererDomainShader&) = delete;
	ModelRendererDomainShader& operator=(const ModelRendererDomainShader&) = delete;
	virtual ~ModelRendererDomainShader();

public:
	static ModelRendererDomainShader* pDomainShader;

public:
	inline static ModelRendererDomainShader* GetInstance() {
		if (pDomainShader == nullptr) {
			pDomainShader = new ModelRendererDomainShader();
		}
		return pDomainShader;
	}

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

