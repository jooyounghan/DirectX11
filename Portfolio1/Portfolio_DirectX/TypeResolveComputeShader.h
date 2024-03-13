#pragma once

#include "IComputeShader.h"

class TypeResolveComputeShader : public IComputeShader
{
private:
	TypeResolveComputeShader();
	TypeResolveComputeShader(const TypeResolveComputeShader&) = delete;
	TypeResolveComputeShader& operator=(const TypeResolveComputeShader&) = delete;
	virtual ~TypeResolveComputeShader();

public:
	static TypeResolveComputeShader* pComputeShader;

public:
	inline static TypeResolveComputeShader* GetInstance() {
		if (pComputeShader == nullptr) {
			pComputeShader = new TypeResolveComputeShader();
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

