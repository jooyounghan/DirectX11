#include "ModelRenderer.h"

#include "AStaticMesh.h"
#include "PBRStaticMesh.h"
#include "AIBLModel.h"

#include "ACamera.h"

#include "ILight.h"
#include "SpotLight.h"
#include "PointLight.h"

#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "Shaders.h"

using namespace std;

ModelRenderer::ModelRenderer()
	: IRenderer(), pCamera(nullptr), pLights(nullptr)
{
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::RenderObjects(
	class ACamera* pCameraIn,
	shared_ptr<AIBLModel> spIBLModelIn,
	const unordered_map<uint32_t, shared_ptr<AStaticMesh>>& vStaticMeshesIn,
	const vector<shared_ptr<ILight>>& vLightsIn
)
{
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_IASETPRIMITIVETOPOLOGY_TOPOLOGY_UNDEFINED);
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();


	pCamera = pCameraIn;
	spIBLModel = spIBLModelIn;
	pLights = &vLightsIn;

	auto [rtvCnt, vRTVs, pDSV] = pCamera->GetRTVs();
	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vRTVs.data(), pDSV);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &pCamera->sViewPort);

	for (auto meshes : vStaticMeshesIn)
	{
		meshes.second->AcceptModelRendering(this);
	}

	pLights = nullptr;
	spIBLModelIn = nullptr;
	pCamera = nullptr;

	vector<ID3D11RenderTargetView*> vNullRTVs;
	vNullRTVs.resize(rtvCnt, nullptr);
	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vNullRTVs.data(), nullptr);

	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_IASETPRIMITIVETOPOLOGY_TOPOLOGY_UNDEFINED);
	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
}


void ModelRenderer::RenderModel(PBRStaticMesh& pbrStaticMesh)
{
#pragma region Preset
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->HSSetShader(shaders.GetHullShader(Shaders::BaseHS), NULL, NULL);
	DirectXDevice::pDeviceContext->DSSetShader(shaders.GetDomainShader(Shaders::BaseDS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));

	auto [uiStrides, uiOffsets, vertexBuffers] = pbrStaticMesh.GetInputInfo();
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, vertexBuffers.data(), uiStrides.data(), uiOffsets.data());
	
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	DirectXDevice::pDeviceContext->IASetIndexBuffer(pbrStaticMesh.spMeshFile->cpInicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pbrStaticMesh.cpTransformationBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, pCamera->cpPositionBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, pbrStaticMesh.cpPBRConstantBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, pbrStaticMesh.cpPBRTextureFlagBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
		pbrStaticMesh.pModelTexture[HEIGHT_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[HEIGHT_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
#pragma endregion

#pragma region Ambient Lighting Preset
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::PBRWithIBLPS), NULL, NULL);

	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, 
		spIBLModel->spEnvSpecularTextureFile ? spIBLModel->spEnvSpecularTextureFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1, 
		spIBLModel->spEnvDiffuseTextureFile ? spIBLModel->spEnvDiffuseTextureFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1, 
		spIBLModel->spEnvBrdfTextureFile ? spIBLModel->spEnvBrdfTextureFile->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1,
		pbrStaticMesh.pModelTexture[AO_TEXUTRE_MAP] ?
		pbrStaticMesh.pModelTexture[AO_TEXUTRE_MAP]->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1,
		pbrStaticMesh.pModelTexture[COLOR_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[COLOR_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1,
		pbrStaticMesh.pModelTexture[METALNESS_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[METALNESS_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(6, 1,
		pbrStaticMesh.pModelTexture[ROUGHNESS_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[ROUGHNESS_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(7, 1,
		pbrStaticMesh.pModelTexture[EMISSION_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[EMISSION_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(8, 1,
		pbrStaticMesh.pModelTexture[NORMAL_TEXTURE_MAP] ?
		pbrStaticMesh.pModelTexture[NORMAL_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pbrStaticMesh.cpIdBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, pCamera->cpPositionBuffer.GetAddressOf());
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
	DirectXDevice::pDeviceContext->PSSetShaderResources(8, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
#pragma endregion
	DirectXDevice::pDeviceContext->OMSetBlendState(DirectXDevice::pAddingBlendState, NULL, UINT(0xFFFFFFFF));
	DirectXDevice::pDeviceContext->OMSetDepthStencilState(DirectXDevice::pDrawLessEqualDSS, 0);
	for (auto& pLight : *pLights)
	{
#pragma region Direct Lighting Preset
		pLight->AcceptSettingForDirectLighting(this);
		DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1,
			pbrStaticMesh.pModelTexture[COLOR_TEXTURE_MAP] ?
			pbrStaticMesh.pModelTexture[COLOR_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1,
			pbrStaticMesh.pModelTexture[METALNESS_TEXTURE_MAP] ?
			pbrStaticMesh.pModelTexture[METALNESS_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1,
			pbrStaticMesh.pModelTexture[ROUGHNESS_TEXTURE_MAP] ?
			pbrStaticMesh.pModelTexture[ROUGHNESS_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1,
			pbrStaticMesh.pModelTexture[EMISSION_TEXTURE_MAP] ?
			pbrStaticMesh.pModelTexture[EMISSION_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1,
			pbrStaticMesh.pModelTexture[NORMAL_TEXTURE_MAP] ?
			pbrStaticMesh.pModelTexture[NORMAL_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);

		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pbrStaticMesh.cpIdBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(4, 1, pbrStaticMesh.cpPBRConstantBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(5, 1, pbrStaticMesh.cpPBRTextureFlagBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(6, 1, pCamera->cpPositionBuffer.GetAddressOf());
		
		DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, DirectXDevice::ppCompareBorderSampler);
#pragma endregion

		pbrStaticMesh.Draw();

#pragma region Direct Lighting Reset
		pLight->AcceptResetingForDirectLighting(this);
		DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1, &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1, &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1, &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1, &pNullSRV);

		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(4, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(5, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(6, 1, &pNullBuffer);

		DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, &pNullSampler);
#pragma endregion
	}

	DirectXDevice::pDeviceContext->OMSetBlendState(nullptr, NULL, UINT(0xFFFFFFFF));
	DirectXDevice::pDeviceContext->OMSetDepthStencilState(nullptr, 0);
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

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, &pNullSampler);
#pragma endregion
}

void ModelRenderer::RenderModel(AIBLModel& iblMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

#pragma region Preset
	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::IBLModelPS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));

	auto [uiStrides, uiOffsets, vertexBuffers] = iblMesh.GetInputInfo();
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, vertexBuffers.data(), uiStrides.data(), uiOffsets.data());

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectXDevice::pDeviceContext->IASetIndexBuffer(iblMesh.spMeshFile->cpInicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	vector<ID3D11ShaderResourceView*> SRVs{
		iblMesh.spEnvSpecularTextureFile != nullptr ? iblMesh.spEnvSpecularTextureFile->GetSRV() : nullptr,
		iblMesh.spEnvDiffuseTextureFile != nullptr ? iblMesh.spEnvDiffuseTextureFile->GetSRV() : nullptr
	};

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, iblMesh.cpTransformationBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, iblMesh.cpIdBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, iblMesh.cpIBLDataBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetShaderResources(0, (UINT)SRVs.size(), SRVs.data());
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
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

	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, pointLight.cpSRV.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, pointLight.cpPositionBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, pointLight.cpBaseLightBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(7, 1, pointLight.viewable[0].cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(8, 1, pointLight.viewable[1].cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(9, 1, pointLight.viewable[2].cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(10, 1, pointLight.viewable[3].cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(11, 1, pointLight.viewable[4].cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(12, 1, pointLight.viewable[5].cpViewProjBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, DirectXDevice::ppCompareClampSampler);
}

void ModelRenderer::SetLight(SpotLight& spotLight)
{
	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::PBRWithSpotPS), NULL, NULL);

	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, spotLight.cpSRV.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, spotLight.cpPositionBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, spotLight.cpBaseLightBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, spotLight.cpSpotLightBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(7, 1, spotLight.cpViewProjBuffer.GetAddressOf());
}

void ModelRenderer::ResetLight(PointLight& pointLight)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(7, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(8, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(9, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(10, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(11, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(12, 1, &pNullBuffer);



	DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, &pNullSampler);
}

void ModelRenderer::ResetLight(SpotLight& pointLight)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);

	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(7, 1, &pNullBuffer);
}

