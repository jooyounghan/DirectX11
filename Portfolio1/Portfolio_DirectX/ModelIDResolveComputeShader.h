#pragma once

#include "IComputeShader.h"

class ModelIDResolveComputeShader : public IComputeShader
{
private:
	ModelIDResolveComputeShader();
	ModelIDResolveComputeShader(const ModelIDResolveComputeShader&) = delete;
	ModelIDResolveComputeShader& operator=(const ModelIDResolveComputeShader&) = delete;
	virtual ~ModelIDResolveComputeShader();

public:
	static ModelIDResolveComputeShader* pComputeShader;

public:
	inline static ModelIDResolveComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new ModelIDResolveComputeShader();
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
