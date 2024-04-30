#pragma once
#include "IShader.h"

struct SingleSourceCSBindingSet
{
	ID3D11ShaderResourceView** ppInputSRV;
	ID3D11UnorderedAccessView** ppOutputUAV;
};

struct DoubleSourceCSBindingSet
{
	ID3D11ShaderResourceView** ppInputSRV1;
	ID3D11ShaderResourceView** ppInputSRV2;
	ID3D11UnorderedAccessView** ppOutputUAV;
};


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

