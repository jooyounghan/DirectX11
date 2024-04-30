#pragma once

#include "IComputeShader.h"

class MS16ToSS8ComputeShader : public IComputeShader
{
private:
	MS16ToSS8ComputeShader();
	MS16ToSS8ComputeShader(const MS16ToSS8ComputeShader&) = delete;
	MS16ToSS8ComputeShader& operator=(const MS16ToSS8ComputeShader&) = delete;
	virtual ~MS16ToSS8ComputeShader();

public:
	static MS16ToSS8ComputeShader* pComputeShader;

public:
	inline static MS16ToSS8ComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new MS16ToSS8ComputeShader();
		}
		return pComputeShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

