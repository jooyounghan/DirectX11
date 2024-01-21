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
	enum EVertexShader : size_t
	{
		BaseVertexShader,
		NumVertexShader
	};
	enum EPixelShader : size_t
	{
		BasePixelShader,
		NumPixelShader
	};
	enum EComputeShader : size_t
	{
		ResolveComputeShader,
		NumComputeShader
	};

public:
	std::vector<Microsoft::WRL::ComPtr<ID3D11VertexShader>> vVertexShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11InputLayout>> vInputLayOuts;
	std::vector<Microsoft::WRL::ComPtr<ID3D11PixelShader>> vPixelShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ComputeShader>> vComputeShaders;

public:
	inline static Shaders& GetInstance()
	{
		static Shaders s = Shaders();
		return s;
	}

public:
	void Init(ID3D11Device* pDeviceIn);

public:
	ID3D11VertexShader* GetVertexShader(IN EVertexShader eVertexShader);
	ID3D11InputLayout* GetInputLayout(IN EVertexShader eVertexShader);
	ID3D11PixelShader* GetPixelShader(IN EPixelShader ePixelShader);
	ID3D11ComputeShader* GetComputeShader(IN EComputeShader eComputeShader);
};

