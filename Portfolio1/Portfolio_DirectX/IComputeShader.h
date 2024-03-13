#pragma once
#include "IShader.h"

class IComputeShader : public IShader
{
protected:
	IComputeShader() {};
	virtual ~IComputeShader() {};

protected:
	static ID3D11UnorderedAccessView* pNullUAV;

protected:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> cpComputeShader;

public:
	virtual void ApplyShader() = 0;
	virtual void DisapplyShader() = 0;
};

