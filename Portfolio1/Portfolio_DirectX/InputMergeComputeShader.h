#pragma once

#include "IComputeShader.h"

class InputMergeComputeShader : public IComputeShader
{
private:
	InputMergeComputeShader();
	InputMergeComputeShader(const InputMergeComputeShader&) = delete;
	InputMergeComputeShader& operator=(const InputMergeComputeShader&) = delete;
	virtual ~InputMergeComputeShader();

public:
	static InputMergeComputeShader* pComputeShader;

public:
	inline static InputMergeComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new InputMergeComputeShader();
		}
		return pComputeShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

