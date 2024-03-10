#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <vector>

class Shaders
{
private:
	Shaders();
	Shaders(const Shaders& ref) {}
	Shaders& operator=(const Shaders& ref) {}
	~Shaders() {}

public:
	enum EComputeShader : size_t
	{
		ResolveCS,
		MS16ToSS8CS,
		TypeResolveCS,
		BlurCS,
		NumComputeShader
	};


private:
	std::vector<Microsoft::WRL::ComPtr<ID3D11ComputeShader>>	vComputeShaders;

public:
	inline static Shaders& GetInstance()
	{
		static Shaders s = Shaders();
		return s;
	}

public:
	void Init(ID3D11Device* pDeviceIn);

public:
	ID3D11ComputeShader* GetComputeShader(IN EComputeShader eComputeShader);

};

