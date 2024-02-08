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
		BaseVS,
		NormalVectorVS,
		NumVertexShader
	};
	enum EPixelShader : size_t
	{
		BasePS,
		DepthOnlyPathPS,
		PBRWithIBLPS,
		PBRWithSpotPS,
		PBRWithPointPS,
		IBLModelPS,
		NormalVectorPS,
		NumPixelShader
	};
	enum EComputeShader : size_t
	{
		ResolveCS,
		MS16ToSS8CS,
		NumComputeShader
	};

	enum EHullShader : size_t
	{
		BaseHS,
		NumHullShader
	};

	enum EDomainShader : size_t
	{
		BaseDS,
		NumDomainShader
	};

	enum EGeometryShader : size_t
	{
		NormalVectorGS,
		NumGeometryShader
	};

public:
	std::vector<Microsoft::WRL::ComPtr<ID3D11VertexShader>>		vVertexShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11InputLayout>>		vInputLayOuts;
	std::vector<Microsoft::WRL::ComPtr<ID3D11PixelShader>>		vPixelShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ComputeShader>>	vComputeShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11HullShader>>		vHullShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11DomainShader>>		vDomainShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11GeometryShader>>	vGeometryShaders;

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
	ID3D11HullShader* GetHullShader(IN EHullShader eHullShader);
	ID3D11DomainShader* GetDomainShader(IN EDomainShader eDomainShader);
	ID3D11GeometryShader* GetGeometryShader(IN EGeometryShader eGeometryShader);
};

