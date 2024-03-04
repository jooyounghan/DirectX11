#pragma once
#include "IShader.h"

class IVertexShader : public IShader
{
public:
	IVertexShader();
	virtual ~IVertexShader();

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> cpInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> cpVertexShader;

public:
	virtual void ApplyShader() = 0;
	virtual void DisapplyShader() = 0;
};

