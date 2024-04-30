#pragma once

#include "IPixelShader.h"

class AIBLMesh;
class PBRStaticMesh;
class Viewable;

struct IBLLightPSBindingSet
{
	AIBLMesh* pIblMesh;
	size_t meshIdx;
	PBRStaticMesh* pPbrStaticMesh;
	Viewable* pViewable;
};

class PBRIBLLightPixelShader : public IPixelShader
{
private:
	PBRIBLLightPixelShader();
	PBRIBLLightPixelShader(const PBRIBLLightPixelShader&) = delete;
	PBRIBLLightPixelShader& operator=(const PBRIBLLightPixelShader&) = delete;
	virtual ~PBRIBLLightPixelShader();

public:
	static PBRIBLLightPixelShader* pPixelShader;

public:
	inline static PBRIBLLightPixelShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new PBRIBLLightPixelShader();
		}
		return pPixelShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};
