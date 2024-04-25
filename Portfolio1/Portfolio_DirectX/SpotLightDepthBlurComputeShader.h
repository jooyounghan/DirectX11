#pragma once

#include "IComputeShader.h"

class SpotLightDepthBlurComputeShader : public IComputeShader
{
private:
	SpotLightDepthBlurComputeShader();
	SpotLightDepthBlurComputeShader(const SpotLightDepthBlurComputeShader&) = delete;
	SpotLightDepthBlurComputeShader& operator=(const SpotLightDepthBlurComputeShader&) = delete;
	virtual ~SpotLightDepthBlurComputeShader();

public:
	static SpotLightDepthBlurComputeShader* pComputeShader;

public:
	inline static SpotLightDepthBlurComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new SpotLightDepthBlurComputeShader();
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
