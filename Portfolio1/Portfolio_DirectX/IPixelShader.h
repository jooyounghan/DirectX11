#pragma once
#include "IShader.h"

class IPixelShader : public IShader
{
protected:
	IPixelShader();
	virtual ~IPixelShader();

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> cpPixelShader;
};

