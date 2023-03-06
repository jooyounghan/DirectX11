#pragma once

#include "Shader.h"

class LightShader : public Shader
{
public:
	LightShader(ComPtr<ID3D11Device>& device);
	~LightShader();
};