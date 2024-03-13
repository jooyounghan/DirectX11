#pragma once

#include "IPixelShader.h"

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

public:
	void SetShader(class AIBLMesh& iblMesh, class Viewable& viewable);
	void ResetShader();
};

