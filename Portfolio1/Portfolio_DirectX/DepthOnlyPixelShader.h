#pragma once

#include "IPixelShader.h"

class DepthOnlyPixelShader : public IPixelShader
{
private:
	DepthOnlyPixelShader();
	DepthOnlyPixelShader(const DepthOnlyPixelShader&) = delete;
	DepthOnlyPixelShader& operator=(const DepthOnlyPixelShader&) = delete;
	virtual ~DepthOnlyPixelShader();

public:
	static DepthOnlyPixelShader* pPixelShader;

public:
	inline static DepthOnlyPixelShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new DepthOnlyPixelShader();
		}
		return pPixelShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

