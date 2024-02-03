#include "Shaders.h"
#include "ID3D11Helper.h"

Shaders::Shaders() {}

void Shaders::Init(ID3D11Device* pDeviceIn)
{
	vVertexShaders.resize(NumVertexShader);
	vInputLayOuts.resize(NumVertexShader);
	vPixelShaders.resize(NumPixelShader);
	vComputeShaders.resize(NumComputeShader);
	
	// Vertex Shader 초기화	
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	ID3D11Helper::CreateVSInputLayOut(pDeviceIn, L"BaseVS.hlsl", vInputElemDesc, vVertexShaders[BaseVS].GetAddressOf(), vInputLayOuts[0].GetAddressOf());

	// Pixel Shader 초기화
	ID3D11Helper::CreatePS(pDeviceIn, L"BasePS.hlsl", vPixelShaders[BasePS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"DepthOnlyPathPS.hlsl", vPixelShaders[DepthOnlyPathPS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"PBRWithSpotPS.hlsl", vPixelShaders[PBRWithSpotPS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"PBRWithPointPS.hlsl", vPixelShaders[PBRWithPointPS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"IBLModelPS.hlsl", vPixelShaders[IBLModelPS].GetAddressOf());


	// Compute Shader 초기화
	ID3D11Helper::CreateCS(pDeviceIn, L"ModelIDResolveCS.hlsl", vComputeShaders[ResolveCS].GetAddressOf());
	ID3D11Helper::CreateCS(pDeviceIn, L"MS16ToSS8CS.hlsl", vComputeShaders[MS16ToSS8CS].GetAddressOf());
}

ID3D11VertexShader* Shaders::GetVertexShader(IN EVertexShader eVertexShader)
{
	if (vVertexShaders.size() > eVertexShader)
	{
		return vVertexShaders[eVertexShader].Get();
	}
	return nullptr;
}

ID3D11InputLayout* Shaders::GetInputLayout(IN EVertexShader eVertexShader)
{
	if (vInputLayOuts.size() > eVertexShader)
	{
		return vInputLayOuts[eVertexShader].Get();
	}
	return nullptr;
}

ID3D11PixelShader* Shaders::GetPixelShader(IN EPixelShader ePixelShader)
{
	if (vPixelShaders.size() > ePixelShader)
	{
		return vPixelShaders[ePixelShader].Get();
	}
	return nullptr;
}

ID3D11ComputeShader* Shaders::GetComputeShader(IN EComputeShader eComputeShader)
{
	if (vComputeShaders.size() > eComputeShader)
	{
		return vComputeShaders[eComputeShader].Get();
	}
	return nullptr;
}
