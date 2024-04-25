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

public:
	void SetShader(
		ID3D11ShaderResourceView** ppInputSRV1, 
		ID3D11ShaderResourceView** ppInputSRV2,
		ID3D11UnorderedAccessView** ppOutputUAV
	);
	void ResetShader();
};

