#include "PointLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "Shaders.h"
#include "AStaticMesh.h"

PointLight::PointLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchRadIn,
	const float& fYawRadIn,
	const float& fRollRadIn
) : 
	ILight(fXPos, fYPos, fZPos), 
	IMovable(fXPos, fYPos, fZPos),
	IAngleAdjustable(fPitchRadIn, fYawRadIn, fRollRadIn),
	viewablesDirections{
		{
			fXPos, fYPos, fZPos,
			fPitchRadIn, DirectX::XMConvertToRadians(-90.f) + fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchRadIn, DirectX::XMConvertToRadians(90.f) + fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			DirectX::XMConvertToRadians(-90.f) + fPitchRadIn, fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			DirectX::XMConvertToRadians(90.f) + fPitchRadIn, fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchRadIn, fYawRadIn, fRollRadIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchRadIn, fYawRadIn + DirectX::XMConvertToRadians(180.f), fRollRadIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		}
}
{
	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		sBaseLightData.fFallOffStart = gDefaultFallOffStart;
		sBaseLightData.fFallOffEnd = gDefaultFallOffEnd;
	}
}

PointLight::~PointLight() {}


void PointLight::UpdateLight(const std::vector<class AStaticMesh*>& pModels)
{
	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		viewablesDirections[idx].fNearZ = sBaseLightData.fFallOffStart;
		viewablesDirections[idx].fFarZ = sBaseLightData.fFallOffEnd;
		viewablesDirections[idx].UpdateView();
	}

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sBaseLightData, D3D11_MAP_WRITE_DISCARD,
		cpBaseLightBuffer.Get()
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

	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, viewablesDirections[idx].cpDSV.Get());
		viewablesDirections[idx].ClearDSV();

		DirectXDevice::pDeviceContext->RSSetViewports(1, &viewablesDirections[idx].sViewPort);

		DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, viewablesDirections[idx].cpViewProjBuffer.GetAddressOf());

		for (auto model : pModels)
		{
			DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, model->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
			DirectXDevice::pDeviceContext->IASetIndexBuffer(model->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, model->cpTransformationBuffer.GetAddressOf());
			DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, model->cpIdBuffer.GetAddressOf());

			model->Draw();
		}
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
