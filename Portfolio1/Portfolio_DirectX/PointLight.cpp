#include "PointLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "Shaders.h"
#include "AStaticMesh.h"

PointLight::PointLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn
) : 
	ILight(fXPos, fYPos, fZPos), 
	IMovable(fXPos, fYPos, fZPos),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn),
	viewablesDirections{
		{
			fXPos, fYPos, fZPos,
			fPitchDegIn, -90.f + fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchDegIn, 90.f + fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			-90.f + fPitchDegIn, fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			90.f + fPitchDegIn, fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchDegIn, fYawDegIn, fRollDegIn,
			DirectX::XMConvertToRadians(gLightFovDeg),
			gDefaultFallOffStart, gDefaultFallOffEnd,
			1000, 1000
		},
		{
			fXPos, fYPos, fZPos,
			fPitchDegIn, fYawDegIn + 180.f, fRollDegIn,
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


void PointLight::UpdateLight(const std::unordered_map<uint32_t, AStaticMesh*>& pModels)
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
			DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, model.second->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
			DirectXDevice::pDeviceContext->IASetIndexBuffer(model.second->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, model.second->cpTransformationBuffer.GetAddressOf());
			DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, model.second->cpIdBuffer.GetAddressOf());

			model.second->Draw();
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
