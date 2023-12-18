#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "ShaderTypeEnum.h"

using namespace DirectX;

SpotLight::SpotLight(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	: LightInterface(pDeviceIn, pDeviceContextIn)
{
	AutoZeroMemory(sSpotLightViewProjData);
	ID3D11Helper::CreateBuffer(
		pDevice, sSpotLightViewProjData,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		NULL, NULL,
		cpSpotLightViewProjDataBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateTexture2D(
		pDevice, 1000.f, 1000.f,
		1, 0, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
		NULL, NULL, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_TYPELESS,
		cpShadowMapTexture.GetAddressOf()
	);
	ID3D11Helper::CreateDepthStencilView(pDevice, cpShadowMapTexture.Get(), cpShadowMapDSV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(pDevice, cpShadowMapTexture.Get(), cpShadowMapSRV.GetAddressOf());
}

SpotLight::~SpotLight()
{
}

void SpotLight::Update()
{
	LightInterface::Update();

	XMMatrix tempAffineTransform = MathematicalHelper::MakeAffineTransformation(1.f, 1.f, 1.f, sBaseLightData.)

	XMMATRIX tempViewProj = MathematicalHelper::MakeViewProjMatrix(sBaseLightData.xmvLocation, xmvDirectDefault, xmvUpDefault, 90.f, 1.f, 0.01f, 1000.f);
	sSpotLightViewProjData.xmmViewProj = XMMatrixTranspose(tempViewProj);
	sSpotLightViewProjData.xmmViewProjInv = XMMatrixInverse(nullptr, tempViewProj);
	ID3D11Helper::UpdateBuffer(pDeviceContext, sSpotLightViewProjData, D3D11_MAP_WRITE_DISCARD, cpSpotLightViewProjDataBuffer.Get());
}

void SpotLight::SetConstantBuffers()
{
}

void SpotLight::ResetConstantBuffers()
{
}

void SpotLight::SetShaderResources()
{
}

void SpotLight::ResetShaderResources()
{
}
