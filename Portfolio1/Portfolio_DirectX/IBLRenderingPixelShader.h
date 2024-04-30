#pragma once

#include "IPixelShader.h"

class AIBLMesh;
class Viewable;

struct IBLPSBindingSet
{
	AIBLMesh* pIblMesh;
	Viewable* pViewable;
};

class IBLRenderingPixelShader : public IPixelShader
{
private:
	IBLRenderingPixelShader();
	IBLRenderingPixelShader(const IBLRenderingPixelShader&) = delete;
	IBLRenderingPixelShader& operator=(const IBLRenderingPixelShader&) = delete;
	virtual ~IBLRenderingPixelShader();

public:
	static IBLRenderingPixelShader* pPixelShader;

public:
	inline static IBLRenderingPixelShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new IBLRenderingPixelShader();
		}
		return pPixelShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

