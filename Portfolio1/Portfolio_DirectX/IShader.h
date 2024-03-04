#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

class IShader
{
public:
	IShader() {};
	~IShader() {};

public:
	virtual void ApplyShader() = 0;
	virtual void DisapplyShader() = 0;
};

