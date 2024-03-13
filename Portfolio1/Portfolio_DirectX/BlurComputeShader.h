#pragma once

#include "IComputeShader.h"

class BlurComputeShader : public IComputeShader
{
private:
	BlurComputeShader();
	BlurComputeShader(const BlurComputeShader&) = delete;
	BlurComputeShader& operator=(const BlurComputeShader&) = delete;
	virtual ~BlurComputeShader();

public:
	static BlurComputeShader* pComputeShader;

public:
	inline static BlurComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new BlurComputeShader();
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
