#include "ModelRenderer.h"

#include "AStaticMesh.h"
#include "APBRStaticMesh.h"
#include "AIBLModel.h"

#include "ACamera.h"

#include "ILight.h"

#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "Shaders.h"

using namespace std;

ModelRenderer::ModelRenderer()
	: IRenderer(), pCamera(nullptr)
{
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::RenderObjects(
	class AIBLModel* pIBLModelIn,
	const std::unordered_map<uint32_t, AStaticMesh*>& vStaticMeshesIn,
	const std::vector<class ILight*>& vLightsIn
)
{
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_IASETPRIMITIVETOPOLOGY_TOPOLOGY_UNDEFINED);
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();


	auto [rtvCnt, vRTVs, pDSV] = pCamera->GetRTVs();
	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vRTVs.data(), pDSV);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &pCamera->sViewPort);

	pIBLModel = pIBLModelIn;
	pLights = &vLightsIn;

	for (auto meshes : vStaticMeshesIn)
	{
		meshes.second->AcceptModelRendering(this);
	}

	pLights = nullptr;
	pIBLModel = nullptr;

	vector<ID3D11RenderTargetView*> vNullRTVs;
	vNullRTVs.resize(rtvCnt, nullptr);
	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vNullRTVs.data(), nullptr);

	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_IASETPRIMITIVETOPOLOGY_TOPOLOGY_UNDEFINED);
	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
}


void ModelRenderer::RenderModel(APBRStaticMesh& pbrStaticMesh)
{
#pragma region Preset
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	UINT uiStride = sizeof(InputLayout);
	UINT uiOffset = 0;

	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->HSSetShader(shaders.GetHullShader(Shaders::BaseHS), NULL, NULL);
	DirectXDevice::pDeviceContext->DSSetShader(shaders.GetDomainShader(Shaders::BaseDS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, pbrStaticMesh.cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
	DirectXDevice::pDeviceContext->IASetIndexBuffer(pbrStaticMesh.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pbrStaticMesh.cpTransformationBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, pCamera->cpPositionBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, pbrStaticMesh.cpPBRConstantBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, pbrStaticMesh.cpPBRTextureFlagBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
		pbrStaticMesh.pModelTexture[APBRStaticMesh::NORMAL_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[APBRStaticMesh::NORMAL_TEXTURE_MAP]->cpSRV.GetAddressOf() : &pNullSRV);
	DirectXDevice::pDeviceContext->DSSetShaderResources(1, 1,
		pbrStaticMesh.pModelTexture[APBRStaticMesh::HEIGHT_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[APBRStaticMesh::HEIGHT_TEXTURE_MAP]->cpSRV.GetAddressOf() : &pNullSRV);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
#pragma endregion

#pragma region Ambient Lighting Preset
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::PBRWithIBLPS), NULL, NULL);

	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, 
		pIBLModel->spEnvSpecularTextureFile ? pIBLModel->spEnvSpecularTextureFile->cpSRV.GetAddressOf() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1, 
		pIBLModel->spEnvDiffuseTextureFile ? pIBLModel->spEnvDiffuseTextureFile->cpSRV.GetAddressOf() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1, 
		pIBLModel->spEnvBrdfTextureFile ? pIBLModel->spEnvBrdfTextureFile->cpSRV.GetAddressOf() : &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1,
		pbrStaticMesh.pModelTexture[APBRStaticMesh::AO_TEXUTRE_MAP] ?
		pbrStaticMesh.pModelTexture[APBRStaticMesh::AO_TEXUTRE_MAP]->cpSRV.GetAddressOf() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1,
		pbrStaticMesh.pModelTexture[APBRStaticMesh::COLOR_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[APBRStaticMesh::COLOR_TEXTURE_MAP]->cpSRV.GetAddressOf() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1,
		pbrStaticMesh.pModelTexture[APBRStaticMesh::METALNESS_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[APBRStaticMesh::METALNESS_TEXTURE_MAP]->cpSRV.GetAddressOf() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(6, 1,
		pbrStaticMesh.pModelTexture[APBRStaticMesh::ROUGHNESS_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[APBRStaticMesh::ROUGHNESS_TEXTURE_MAP]->cpSRV.GetAddressOf() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(7, 1,
		pbrStaticMesh.pModelTexture[APBRStaticMesh::EMISSION_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[APBRStaticMesh::EMISSION_TEXTURE_MAP]->cpSRV.GetAddressOf() : &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pbrStaticMesh.cpIdBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, pbrStaticMesh.cpPositionBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, pbrStaticMesh.cpPBRConstantBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, pbrStaticMesh.cpPBRTextureFlagBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
#pragma endregion

	pbrStaticMesh.Draw();

#pragma region Ambient Lighting Reset
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);

	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(6, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(7, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
#pragma endregion


	for (ILight* pLight : *pLights)
	{
#pragma region Preset
		pLight->AcceptSettingForDirectLighting(this);
#pragma endregion

		pbrStaticMesh.Draw();

#pragma region Reset
		pLight->AcceptResetingForDirectLighting(this);
#pragma endregion
	}

#pragma region Reset
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->HSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->DSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->DSSetShaderResources(1, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, &pNullSampler);
#pragma endregion
}

void ModelRenderer::RenderModel(AIBLModel& iblMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

#pragma region Preset
	UINT uiStride = sizeof(InputLayout);
	UINT uiOffset = 0;

	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::IBLModelPS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, iblMesh.cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
	DirectXDevice::pDeviceContext->IASetIndexBuffer(iblMesh.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	vector<ID3D11ShaderResourceView*> SRVs{
		iblMesh.spEnvSpecularTextureFile != nullptr ? iblMesh.spEnvSpecularTextureFile->cpSRV.Get() : nullptr,
		iblMesh.spEnvDiffuseTextureFile != nullptr ? iblMesh.spEnvDiffuseTextureFile->cpSRV.Get() : nullptr
	};

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, iblMesh.cpTransformationBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, iblMesh.cpIdBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, iblMesh.cpIBLDataBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetShaderResources(0, (UINT)SRVs.size(), SRVs.data());
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
#pragma endregion

	iblMesh.Draw();

#pragma region Reset
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);

	vector<ID3D11ShaderResourceView*> vNullSRVs;
	vNullSRVs.resize(SRVs.size(), nullptr);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetShaderResources(0, (UINT)vNullSRVs.size(), vNullSRVs.data());
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
#pragma endregion
}

void ModelRenderer::SetLight(PointLight& pointLight)
{
	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::PBRWithPointPS), NULL, NULL);
}

void ModelRenderer::SetLight(SpotLight& spotLight)
{
	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::PBRWithPointPS), NULL, NULL);
}

void ModelRenderer::ResetLight(PointLight& pointLight)
{
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
}

void ModelRenderer::ResetLight(SpotLight& pointLight)
{
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
}

