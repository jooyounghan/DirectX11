#include "Shaders.h"
#include "ID3D11Helper.h"

Shaders::Shaders() {}

void Shaders::Init(ID3D11Device* pDeviceIn)
{
	vComputeShaders.resize(NumComputeShader);

	// Compute Shader ÃÊ±âÈ­
	ID3D11Helper::CreateCS(pDeviceIn, L"ModelIDResolveCS.hlsl", vComputeShaders[ResolveCS].GetAddressOf());
	ID3D11Helper::CreateCS(pDeviceIn, L"MS16ToSS8CS.hlsl", vComputeShaders[MS16ToSS8CS].GetAddressOf());
	ID3D11Helper::CreateCS(pDeviceIn, L"TypeResolveCS.hlsl", vComputeShaders[TypeResolveCS].GetAddressOf());
	ID3D11Helper::CreateCS(pDeviceIn, L"BlurCS.hlsl", vComputeShaders[BlurCS].GetAddressOf());
}

ID3D11ComputeShader* Shaders::GetComputeShader(IN EComputeShader eComputeShader)
{
	if (vComputeShaders.size() > eComputeShader)
	{
		return vComputeShaders[eComputeShader].Get();
	}
	return nullptr;
}
