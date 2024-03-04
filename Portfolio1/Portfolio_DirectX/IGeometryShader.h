#pragma once
#include "IShader.h"

class IGeometryShader : public IShader
{
public:
	IGeometryShader();
	virtual ~IGeometryShader();

protected:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> cpGeometryShader;

public:
	virtual void ApplyShader() = 0;
	virtual void DisapplyShader() = 0;
};

