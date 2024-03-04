#pragma once
#include "IShader.h"

class IPixelShader : public IShader
{
public:
	IPixelShader();
	virtual ~IPixelShader();

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> cpPixelShader;

public:
	virtual void ApplyShader() = 0;
	virtual void DisapplyShader() = 0;
};

