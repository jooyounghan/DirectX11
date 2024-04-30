#pragma once
#include "IPixelShader.h"

class NormalVectorPixelShader : public IPixelShader
{
private:
	NormalVectorPixelShader();
	NormalVectorPixelShader(const NormalVectorPixelShader&) = delete;
	NormalVectorPixelShader& operator=(const NormalVectorPixelShader&) = delete;
	virtual ~NormalVectorPixelShader();

public:
	static NormalVectorPixelShader* pPixelShader;

public:
	inline static NormalVectorPixelShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new NormalVectorPixelShader();
		}
		return pPixelShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

