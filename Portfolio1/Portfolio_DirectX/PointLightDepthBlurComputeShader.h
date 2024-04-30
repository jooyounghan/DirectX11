#pragma once

#include "IComputeShader.h"

struct PointDepthBlurCSBindingSet
{
	ID3D11ShaderResourceView** ppInputSRV;
	ID3D11UnorderedAccessView** ppOutputUAV;
	size_t	uiIndex;
};

class PointLightDepthBlurComputeShader : public IComputeShader
{
private:
	PointLightDepthBlurComputeShader();
	PointLightDepthBlurComputeShader(const PointLightDepthBlurComputeShader&) = delete;
	PointLightDepthBlurComputeShader& operator=(const PointLightDepthBlurComputeShader&) = delete;
	virtual ~PointLightDepthBlurComputeShader();

public:
	static PointLightDepthBlurComputeShader* pComputeShader;

public:
	inline static PointLightDepthBlurComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new PointLightDepthBlurComputeShader();
		}
		return pComputeShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;

protected:
	static ID3D11ShaderResourceView* pNullSRVs[6];
	static ID3D11UnorderedAccessView* pNullUAVs[6];
};
