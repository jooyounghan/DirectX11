#pragma once
#include "IShader.h"

class IHullShader : public IShader
{
public:
	IHullShader();
	virtual ~IHullShader();

protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader> cpHullShader;
};
