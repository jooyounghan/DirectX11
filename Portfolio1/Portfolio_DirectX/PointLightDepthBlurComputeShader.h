#pragma once

#include "IComputeShader.h"

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

protected:
	static ID3D11ShaderResourceView* pNullSRVs[6];
	static ID3D11UnorderedAccessView* pNullUAVs[6];
public:
	void SetShader(ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV, size_t uiCount);
	void ResetShader();
};
