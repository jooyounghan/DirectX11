#pragma once

#include "IDomainShader.h"

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

public:
	void SetShader(
		const size_t& meshIdx, 
		class PBRStaticMesh& pbrStaticMesh, 
		class Viewable& viewable
	);
	void SetShader(class Viewable& viewable);
	void ResetShader();

};

