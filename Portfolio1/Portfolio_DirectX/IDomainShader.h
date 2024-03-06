#pragma once
#include "IShader.h"

class IDomainShader : public IShader
{
public:
	IDomainShader();
	virtual ~IDomainShader();

protected:
	Microsoft::WRL::ComPtr<ID3D11DomainShader> cpDomainShader;
};