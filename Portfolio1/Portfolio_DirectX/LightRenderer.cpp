
#include "LightRenderer.h"

#include "AStaticMesh.h"

#include "ILight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "APBRStaticMesh.h"
#include "AIBLModel.h"

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
	const std::unordered_map<uint32_t, std::shared_ptr<AStaticMesh>>& vStaticMeshes,
	const std::vector<std::shared_ptr<ILight>>& vLights
)
{
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
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
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
}

void LightRenderer::RenderLightMap(PointLight& pointLight)
{
	ID3D11RenderTargetView* pNullRTV = nullptr;
	ID3D11Buffer* pNullBuffer = nullptr;

	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->HSSetShader(shaders.GetHullShader(Shaders::BaseHS), NULL, NULL);
	DirectXDevice::pDeviceContext->DSSetShader(shaders.GetDomainShader(Shaders::BaseDS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::DepthOnlyPathPS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, pointLight.GetDSVAddress(idx));
		DirectXDevice::pDeviceContext->ClearDepthStencilView(pointLight.GetDSVAddress(idx), D3D11_CLEAR_DEPTH, 1.f, NULL);

		Viewable& current_view = pointLight.viewable[idx];
		DirectXDevice::pDeviceContext->RSSetViewports(1, &current_view.sViewPort);

		DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, current_view.cpViewProjBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, pointLight.cpPositionBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, current_view.cpViewProjBuffer.GetAddressOf());

		for (auto model : *pModelSet)
		{
			model.second->AcceptLightMapUpdating(this);
		}

		DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);


		DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, &pNullBuffer);
	}

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->HSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->DSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);
}

void LightRenderer::RenderLightMap(SpotLight& spotLight)
{
	ID3D11RenderTargetView* pNullRTV = nullptr;
	ID3D11Buffer* pNullBuffer = nullptr;

	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->HSSetShader(shaders.GetHullShader(Shaders::BaseHS), NULL, NULL);
	DirectXDevice::pDeviceContext->DSSetShader(shaders.GetDomainShader(Shaders::BaseDS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::DepthOnlyPathPS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, spotLight.cpDSV.Get());
	spotLight.ClearDSV();

	DirectXDevice::pDeviceContext->RSSetViewports(1, &spotLight.sViewPort);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, spotLight.cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, spotLight.cpPositionBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, spotLight.cpViewProjBuffer.GetAddressOf());


	for (auto model : *pModelSet)
	{
		model.second->AcceptLightMapUpdating(this);
	}

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->HSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->DSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);

	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);


	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, &pNullBuffer);
}

void LightRenderer::SetModelSettingForLightMap(APBRStaticMesh& pbrStaticMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	auto [uiStrides, uiOffsets, vertexBuffers] = pbrStaticMesh.GetInputInfo();
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, vertexBuffers.data(), uiStrides.data(), uiOffsets.data());

	DirectXDevice::pDeviceContext->IASetIndexBuffer(pbrStaticMesh.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pbrStaticMesh.cpTransformationBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, pbrStaticMesh.cpPBRConstantBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, pbrStaticMesh.cpPBRTextureFlagBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
		pbrStaticMesh.pModelTexture[HEIGHT_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[HEIGHT_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);

	pbrStaticMesh.Draw();

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, &pNullSampler);
}

void LightRenderer::SetModelSettingForLightMap(AIBLModel& iblMesh)
{
	// Do Nothing
}

