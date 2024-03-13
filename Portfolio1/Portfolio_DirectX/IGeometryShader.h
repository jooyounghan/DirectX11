#pragma once
#include "IShader.h"

class IGeometryShader : public IShader
{
protected:
	IGeometryShader();
	virtual ~IGeometryShader();

protected:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> cpGeometryShader;
};

