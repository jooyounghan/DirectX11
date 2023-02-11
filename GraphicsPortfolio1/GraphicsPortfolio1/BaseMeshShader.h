#pragma once

#include "Shader.h"

class BaseMeshShader : public Shader
{
public:
	BaseMeshShader(ComPtr<ID3D11Device>& device, const wstring& vs_name, const wstring& ps_name);
	~BaseMeshShader();
};

