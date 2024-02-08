#include "Shaders.h"
#include "ID3D11Helper.h"

Shaders::Shaders() {}

void Shaders::Init(ID3D11Device* pDeviceIn)
{
	vVertexShaders.resize(NumVertexShader);
	vInputLayOuts.resize(NumVertexShader);
	vPixelShaders.resize(NumPixelShader);
	vComputeShaders.resize(NumComputeShader);
	vHullShaders.resize(NumHullShader);
	vDomainShaders.resize(NumDomainShader);
	vGeometryShaders.resize(NumGeometryShader);

	// Vertex Shader 초기화	
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	ID3D11Helper::CreateVSInputLayOut(pDeviceIn, L"BaseVS.hlsl", vInputElemDesc, vVertexShaders[BaseVS].GetAddressOf(), vInputLayOuts[0].GetAddressOf());
	ID3D11Helper::CreateVS(pDeviceIn, L"NormalVectorVS.hlsl", vVertexShaders[NormalVectorVS].GetAddressOf());

	// Pixel Shader 초기화
	ID3D11Helper::CreatePS(pDeviceIn, L"BasePS.hlsl", vPixelShaders[BasePS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"DepthOnlyPathPS.hlsl", vPixelShaders[DepthOnlyPathPS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"PBRWithIBLPS.hlsl", vPixelShaders[PBRWithIBLPS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"PBRWithSpotPS.hlsl", vPixelShaders[PBRWithSpotPS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"PBRWithPointPS.hlsl", vPixelShaders[PBRWithPointPS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"IBLModelPS.hlsl", vPixelShaders[IBLModelPS].GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"NormalVectorPS.hlsl", vPixelShaders[NormalVectorPS].GetAddressOf());

	// Compute Shader 초기화
	ID3D11Helper::CreateCS(pDeviceIn, L"ModelIDResolveCS.hlsl", vComputeShaders[ResolveCS].GetAddressOf());
	ID3D11Helper::CreateCS(pDeviceIn, L"MS16ToSS8CS.hlsl", vComputeShaders[MS16ToSS8CS].GetAddressOf());

	// Hull Shader 초기화
	ID3D11Helper::CreateHS(pDeviceIn, L"BaseHS.hlsl", vHullShaders[BaseHS].GetAddressOf());

	// Domain Shader 초기화
	ID3D11Helper::CreateDS(pDeviceIn, L"BaseDS.hlsl", vDomainShaders[BaseDS].GetAddressOf());

	// Geometry Shader 초기화
	ID3D11Helper::CreateGS(pDeviceIn, L"NormalVectorGS.hlsl", vGeometryShaders[NormalVectorGS].GetAddressOf());
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

ID3D11HullShader* Shaders::GetHullShader(IN EHullShader eHullShader)
{
	if (vHullShaders.size() > eHullShader)
	{
		return vHullShaders[eHullShader].Get();
	}
	return nullptr;
}

ID3D11DomainShader* Shaders::GetDomainShader(IN EDomainShader eDomainShader)
{
	if (vDomainShaders.size() > eDomainShader)
	{
		return vDomainShaders[eDomainShader].Get();
	}
	return nullptr;
}

ID3D11GeometryShader* Shaders::GetGeometryShader(IN EGeometryShader eGeometryShader)
{
	if (vGeometryShaders.size() > eGeometryShader)
	{
		return vGeometryShaders[eGeometryShader].Get();
	}
	return nullptr;
}
