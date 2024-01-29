#include "SpotLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "Shaders.h"
#include "AStaticMesh.h"

SpotLight::SpotLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn
)
	: ILight(fXPos, fYPos, fZPos),
	ViewableDepthOnly(
		fXPos, fYPos, fZPos, 
		fPitchDegIn, fYawDegIn, fRollDegIn,
		DirectX::XMConvertToRadians(gLightFovDeg),
		gDefaultFallOffStart, gDefaultFallOffEnd,
		1000, 1000
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		fPitchDegIn, fYawDegIn, fRollDegIn,
		1000.f, 
		1000.f, 
		DirectX::XMConvertToRadians(90.f),
		gDefaultFallOffStart, gDefaultFallOffEnd
	),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(1000, 1000)
{
	sBaseLightData.fFallOffStart = gDefaultFallOffStart;
	sBaseLightData.fFallOffEnd = gDefaultFallOffEnd;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice,
		sSpotLightData, D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE,
		NULL, cpSpotLightBuffer.GetAddressOf()
	);
}

SpotLight::~SpotLight()
{

}

void SpotLight::UpdateLight(const std::unordered_map<uint32_t, AStaticMesh*>& pModels)
{
	fNearZ = sBaseLightData.fFallOffStart;
	fFarZ = sBaseLightData.fFallOffEnd;

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sBaseLightData, D3D11_MAP_WRITE_DISCARD,
		cpBaseLightBuffer.Get()
	);

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sSpotLightData, D3D11_MAP_WRITE_DISCARD, 
		cpSpotLightBuffer.Get()
	);

	ID3D11RenderTargetView* pNullRTV = nullptr;
	ID3D11Buffer* pNullBuffer = nullptr;
	UINT uiStride = sizeof(InputLayout);
	UINT uiOffset = 0;

	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::DepthOnlyPathPS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, cpDSV.Get());
	ClearDSV();

	DirectXDevice::pDeviceContext->RSSetViewports(1, &sViewPort);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, cpViewProjBuffer.GetAddressOf());

	for (auto model : pModels)
	{
		DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, model.second->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
		DirectXDevice::pDeviceContext->IASetIndexBuffer(model.second->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, model.second->cpTransformationBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, model.second->cpIdBuffer.GetAddressOf());

		model.second->Draw();
	}

	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
}