#include "PointLight.h"
#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"

PointLight::PointLight(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	: LightInterface(pDeviceIn, pDeviceContextIn)
{
	sBaseLightData.uiLightType = ELightType::PointLight;

	ID3D11Helper::CreateBuffer(
		pDevice, sPointLightData, 
		D3D11_USAGE_DEFAULT, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, NULL, 
		cpPointLightDataBuffer.GetAddressOf()
	);
}

PointLight::~PointLight()
{
}

void PointLight::Update()
{
	LightInterface::Update();
	ID3D11Helper::UpdateBuffer(pDeviceContext, sPointLightData, D3D11_MAP_WRITE_DISCARD, cpPointLightDataBuffer.Get());
}

void PointLight::SetConstantBuffers()
{
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, cpBaseLightDataBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTADDITIONAL, 1, cpPointLightDataBuffer.GetAddressOf());
}

void PointLight::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTBASE, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_CBUFF_LIGHTADDITIONAL, 1, &pResetBuffer);
}

void PointLight::SetShaderResources()
{
}

void PointLight::ResetShaderResources()
{
}
