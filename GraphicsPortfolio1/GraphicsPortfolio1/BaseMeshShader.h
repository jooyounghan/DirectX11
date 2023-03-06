#pragma once

#include "Shader.h"

class BaseMeshShader : public Shader
{
public:
	BaseMeshShader(ComPtr<ID3D11Device>& device);
	~BaseMeshShader();
};

