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

protected:
	static ID3D11Buffer* const pNullBuffer;
	static ID3D11ShaderResourceView* const pNullSRV;
	static ID3D11SamplerState* const pNullSampler;

public:
	virtual void ApplyShader() = 0;
	virtual void DisapplyShader() = 0;
};

