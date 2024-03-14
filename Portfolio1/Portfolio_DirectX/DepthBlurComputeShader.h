#pragma once

#include "IComputeShader.h"

class DepthBlurComputeShader : public IComputeShader
{
private:
	DepthBlurComputeShader();
	DepthBlurComputeShader(const DepthBlurComputeShader&) = delete;
	DepthBlurComputeShader& operator=(const DepthBlurComputeShader&) = delete;
	virtual ~DepthBlurComputeShader();

public:
	static DepthBlurComputeShader* pComputeShader;

public:
	inline static DepthBlurComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new DepthBlurComputeShader();
		}
		return pComputeShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV);
	void ResetShader();
};
