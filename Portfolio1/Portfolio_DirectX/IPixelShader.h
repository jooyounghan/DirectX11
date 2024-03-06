#pragma once
#include "IShader.h"

class IPixelShader : public IShader
{
public:
	IPixelShader();
	virtual ~IPixelShader();

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> cpPixelShader;
};

