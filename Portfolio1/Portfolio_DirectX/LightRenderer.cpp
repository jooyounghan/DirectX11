#include "LightRenderer.h"

#include "AStaticMesh.h"

#include "ILight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "DirectXDevice.h"
#include "Shaders.h"

LightRenderer::LightRenderer()
	: IRenderer(), pModelSet(nullptr)
{
}

LightRenderer::~LightRenderer()
{
}

void LightRenderer::UpdateLightMap(
	const std::unordered_map<uint32_t, class AStaticMesh*>& vStaticMeshes,
	const std::vector<std::shared_ptr<ILight>>& vLights
)
{
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();

	pModelSet = &vStaticMeshes;
	Shaders& shaders = Shaders::GetInstance();
	ID3D11VertexShader* pVS = shaders.GetVertexShader(Shaders::EVertexShader::BaseVS);
	ID3D11PixelShader* pPS = shaders.GetPixelShader(Shaders::EPixelShader::DepthOnlyPathPS);
	for (auto& pLight : vLights)
	{
		pLight->AcceptLightRenderer(this);
	}
	pModelSet = nullptr;

	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
}

void LightRenderer::VisitLight(PointLight& pointLight)
{
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
		DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, pointLight.viewablesDirections[idx].cpDSV.Get());
		pointLight.viewablesDirections[idx].ClearDSV();

		DirectXDevice::pDeviceContext->RSSetViewports(1, &pointLight.viewablesDirections[idx].sViewPort);

		DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pointLight.viewablesDirections[idx].cpViewProjBuffer.GetAddressOf());

		for (auto model : *pModelSet)
		{
			AStaticMesh* pModel = model.second;
			DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, pModel->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
			DirectXDevice::pDeviceContext->IASetIndexBuffer(pModel->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pModel->cpTransformationBuffer.GetAddressOf());
			DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pModel->cpIdBuffer.GetAddressOf());
			pModel->Draw();
		}
	}

	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
}

void LightRenderer::VisitLight(SpotLight& spotLight)
{
	ID3D11RenderTargetView* pNullRTV = nullptr;
	ID3D11Buffer* pNullBuffer = nullptr;
	UINT uiStride = sizeof(InputLayout);
	UINT uiOffset = 0;

	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::DepthOnlyPathPS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, spotLight.cpDSV.Get());
	spotLight.ClearDSV();

	DirectXDevice::pDeviceContext->RSSetViewports(1, &spotLight.sViewPort);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, spotLight.cpViewProjBuffer.GetAddressOf());

	for (auto model : *pModelSet)
	{
		AStaticMesh* pModel = model.second;
		DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, pModel->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
		DirectXDevice::pDeviceContext->IASetIndexBuffer(pModel->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pModel->cpTransformationBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pModel->cpIdBuffer.GetAddressOf());
		pModel->Draw();
	}

	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
}

