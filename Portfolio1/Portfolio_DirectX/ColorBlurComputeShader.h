#pragma once

#include "IComputeShader.h"

class ColorBlurComputeShader : public IComputeShader
{
private:
	ColorBlurComputeShader();
	ColorBlurComputeShader(const ColorBlurComputeShader&) = delete;
	ColorBlurComputeShader& operator=(const ColorBlurComputeShader&) = delete;
	virtual ~ColorBlurComputeShader();

public:
	static ColorBlurComputeShader* pComputeShader;

public:
	inline static ColorBlurComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new ColorBlurComputeShader();
		}
		return pComputeShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};
