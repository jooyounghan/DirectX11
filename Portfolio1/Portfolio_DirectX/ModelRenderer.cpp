#include "ModelRenderer.h"

#include "ACamera.h"

#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"

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
	shared_ptr<AIBLMesh> spIBLModelIn,
	const unordered_map<uint32_t, shared_ptr<IMesh>>& vMeshes,
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

	for (auto meshes : vMeshes)
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


void ModelRenderer::RenderModel(SinglePBRModel& singlePBRMesh)
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

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, singlePBRMesh.GetTransformationBuffer());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->GetViewProjBuffer());

	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, pCamera->GetPositionBuffer());

	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, singlePBRMesh.GetPBRConstantBuffer());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, pCamera->GetViewProjBuffer());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, singlePBRMesh.GetPBRTextureFlagBuffer());

	shared_ptr<IImageFile>& heightFile = singlePBRMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);
	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
		heightFile.get() != nullptr ?
		heightFile->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
#pragma endregion

#pragma region Ambient Lighting Preset
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::PBRWithIBLPS), NULL, NULL);

	IImageFile* pSpecularFile = spIBLModel->GetSpecularTextureFileRef().get();
	IImageFile* pDiffuseFile = spIBLModel->GetDiffuseTextureFileRef().get();
	IImageFile* pBRDFFile = spIBLModel->GetBRDFTextureFileRef().get();


	DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, 
		pSpecularFile != nullptr ? pSpecularFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1, 
		pDiffuseFile != nullptr ? pDiffuseFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1, 
		pBRDFFile != nullptr ? pBRDFFile->GetAddressOfSRV() : &pNullSRV);

	shared_ptr<IImageFile>& aoFile = singlePBRMesh.GetTextureImageFileRef(AO_TEXUTRE_MAP);
	shared_ptr<IImageFile>& colorFile = singlePBRMesh.GetTextureImageFileRef(COLOR_TEXTURE_MAP);
	shared_ptr<IImageFile>& metalnessFile = singlePBRMesh.GetTextureImageFileRef(METALNESS_TEXTURE_MAP);
	shared_ptr<IImageFile>& roughnessFile = singlePBRMesh.GetTextureImageFileRef(ROUGHNESS_TEXTURE_MAP);
	shared_ptr<IImageFile>& emissionFile = singlePBRMesh.GetTextureImageFileRef(EMISSION_TEXTURE_MAP);
	shared_ptr<IImageFile>& normalFile = singlePBRMesh.GetTextureImageFileRef(NORMAL_TEXTURE_MAP);

	DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1,
		aoFile.get() != nullptr ?
		aoFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1,
		colorFile.get() != nullptr ?
		colorFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1,
		metalnessFile.get() != nullptr ?
		metalnessFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(6, 1,
		roughnessFile.get() != nullptr ?
		roughnessFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(7, 1,
		emissionFile.get() != nullptr ?
		emissionFile->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->PSSetShaderResources(8, 1,
		normalFile.get() != nullptr ?
		normalFile->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, singlePBRMesh.GetIDBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, pCamera->GetPositionBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, singlePBRMesh.GetPBRConstantBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, singlePBRMesh.GetPBRTextureFlagBuffer());

	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
	DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, DirectXDevice::ppClampSampler);
#pragma endregion

	singlePBRMesh.Draw();

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
	DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, &pNullSampler);
#pragma endregion
	DirectXDevice::pDeviceContext->OMSetBlendState(DirectXDevice::pAddingBlendState, NULL, UINT(0xFFFFFFFF));
	DirectXDevice::pDeviceContext->OMSetDepthStencilState(DirectXDevice::pDrawLessEqualDSS, 0);
	for (auto& pLight : *pLights)
	{
#pragma region Direct Lighting Preset
		pLight->AcceptSettingForDirectLighting(this);
		DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1,
			colorFile.get() != nullptr ?
			colorFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1,
			metalnessFile.get() != nullptr ?
			metalnessFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1,
			roughnessFile.get() != nullptr ?
			roughnessFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1,
			emissionFile.get() != nullptr ?
			emissionFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1,
			normalFile.get() != nullptr ?
			normalFile->GetAddressOfSRV() : &pNullSRV);

		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, singlePBRMesh.GetIDBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(4, 1, singlePBRMesh.GetPBRConstantBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(5, 1, singlePBRMesh.GetPBRTextureFlagBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(6, 1, pCamera->GetPositionBuffer());
		
		DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, DirectXDevice::ppClampSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, DirectXDevice::ppCompareBorderSampler);
#pragma endregion

		singlePBRMesh.Draw();

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
		DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, &pNullSampler);
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

void ModelRenderer::RenderModel(GroupPBRModel& groupPBRMesh)
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

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, groupPBRMesh.GetTransformationBuffer());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->GetViewProjBuffer());
	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, pCamera->GetPositionBuffer());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, pCamera->GetViewProjBuffer());
	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);

	IImageFile* pSpecularFile = spIBLModel->GetSpecularTextureFileRef().get();
	IImageFile* pDiffuseFile = spIBLModel->GetDiffuseTextureFileRef().get();
	IImageFile* pBRDFFile = spIBLModel->GetBRDFTextureFileRef().get();

	vector<PBRStaticMesh>& vPBRMeshes = groupPBRMesh.GetChildrenMeshesRef();
	for (auto& pbrMesh : vPBRMeshes)
	{
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, pbrMesh.GetPBRConstantBuffer());
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, pbrMesh.GetPBRTextureFlagBuffer());
		shared_ptr<IImageFile>& heightFile = pbrMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);
		DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
			heightFile.get() != nullptr ?
			heightFile->GetAddressOfSRV() : &pNullSRV);

		shared_ptr<IImageFile>& aoFile = pbrMesh.GetTextureImageFileRef(AO_TEXUTRE_MAP);
		shared_ptr<IImageFile>& colorFile = pbrMesh.GetTextureImageFileRef(COLOR_TEXTURE_MAP);
		shared_ptr<IImageFile>& metalnessFile = pbrMesh.GetTextureImageFileRef(METALNESS_TEXTURE_MAP);
		shared_ptr<IImageFile>& roughnessFile = pbrMesh.GetTextureImageFileRef(ROUGHNESS_TEXTURE_MAP);
		shared_ptr<IImageFile>& emissionFile = pbrMesh.GetTextureImageFileRef(EMISSION_TEXTURE_MAP);
		shared_ptr<IImageFile>& normalFile = pbrMesh.GetTextureImageFileRef(NORMAL_TEXTURE_MAP);

		DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::PBRWithIBLPS), NULL, NULL);
		DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1,
			pSpecularFile != nullptr ? pSpecularFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1,
			pDiffuseFile != nullptr ? pDiffuseFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1,
			pBRDFFile != nullptr ? pBRDFFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1,
			aoFile.get() != nullptr ?
			aoFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1,
			colorFile.get() != nullptr ?
			colorFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1,
			metalnessFile.get() != nullptr ?
			metalnessFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(6, 1,
			roughnessFile.get() != nullptr ?
			roughnessFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(7, 1,
			emissionFile.get() != nullptr ?
			emissionFile->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->PSSetShaderResources(8, 1,
			normalFile.get() != nullptr ?
			normalFile->GetAddressOfSRV() : &pNullSRV);

		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, groupPBRMesh.GetIDBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, pCamera->GetPositionBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, pbrMesh.GetPBRConstantBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, pbrMesh.GetPBRTextureFlagBuffer());

		DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, DirectXDevice::ppClampSampler);

		pbrMesh.Draw();

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
		DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, &pNullSampler);

		DirectXDevice::pDeviceContext->OMSetBlendState(DirectXDevice::pAddingBlendState, NULL, UINT(0xFFFFFFFF));
		DirectXDevice::pDeviceContext->OMSetDepthStencilState(DirectXDevice::pDrawLessEqualDSS, 0);

		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, groupPBRMesh.GetIDBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(4, 1, pbrMesh.GetPBRConstantBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(5, 1, pbrMesh.GetPBRTextureFlagBuffer());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(6, 1, pCamera->GetPositionBuffer());

		DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, DirectXDevice::ppClampSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, DirectXDevice::ppCompareBorderSampler);

		for (auto& pLight : *pLights)
		{
			pLight->AcceptSettingForDirectLighting(this);
			DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1,
				colorFile.get() != nullptr ?
				colorFile->GetAddressOfSRV() : &pNullSRV);
			DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1,
				metalnessFile.get() != nullptr ?
				metalnessFile->GetAddressOfSRV() : &pNullSRV);
			DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1,
				roughnessFile.get() != nullptr ?
				roughnessFile->GetAddressOfSRV() : &pNullSRV);
			DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1,
				emissionFile.get() != nullptr ?
				emissionFile->GetAddressOfSRV() : &pNullSRV);
			DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1,
				normalFile.get() != nullptr ?
				normalFile->GetAddressOfSRV() : &pNullSRV);

			pbrMesh.Draw();

			pLight->AcceptResetingForDirectLighting(this);

			DirectXDevice::pDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
			DirectXDevice::pDeviceContext->PSSetShaderResources(1, 1, &pNullSRV);
			DirectXDevice::pDeviceContext->PSSetShaderResources(2, 1, &pNullSRV);
			DirectXDevice::pDeviceContext->PSSetShaderResources(3, 1, &pNullSRV);
			DirectXDevice::pDeviceContext->PSSetShaderResources(4, 1, &pNullSRV);
		}

		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(4, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(5, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(6, 1, &pNullBuffer);

		DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(1, 1, &pNullSampler);
		DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, &pNullSampler);

		DirectXDevice::pDeviceContext->OMSetBlendState(nullptr, NULL, UINT(0xFFFFFFFF));
		DirectXDevice::pDeviceContext->OMSetDepthStencilState(nullptr, 0);

		DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1, &pNullSRV);
	}

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->HSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->DSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, &pNullSampler);
}

void ModelRenderer::RenderModel(AIBLMesh& iblMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

#pragma region Preset
	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::IBLModelPS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	IImageFile* pSpecularFile = iblMesh.GetSpecularTextureFileRef().get();
	IImageFile* pDiffuseFile = iblMesh.GetDiffuseTextureFileRef().get();

	vector<ID3D11ShaderResourceView*> SRVs{
		pSpecularFile != nullptr ? pSpecularFile->GetSRV() : nullptr,
		pDiffuseFile != nullptr ? pDiffuseFile->GetSRV() : nullptr
	};

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, iblMesh.GetTransformationBuffer());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->GetViewProjBuffer());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, iblMesh.GetIDBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, iblMesh.GetIBLDataBuffer());

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

	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, pointLight.GetAddressOfSRV());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, pointLight.GetPositionBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, pointLight.GetBaseLightBuffer());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(7, 1, pointLight.GetViewable(PointXDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(8, 1, pointLight.GetViewable(PointNegXDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(9, 1, pointLight.GetViewable(PointYDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(10, 1, pointLight.GetViewable(PointNegYDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(11, 1, pointLight.GetViewable(PointZDirection).GetViewProjBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(12, 1, pointLight.GetViewable(PointNegZDirection).GetViewProjBuffer());

	DirectXDevice::pDeviceContext->PSSetSamplers(2, 1, DirectXDevice::ppCompareClampSampler);
}

void ModelRenderer::SetLight(SpotLight& spotLight)
{
	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::PBRWithSpotPS), NULL, NULL);

	DirectXDevice::pDeviceContext->PSSetShaderResources(5, 1, spotLight.GetAddressOfSRV());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(1, 1, spotLight.GetPositionBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(2, 1, spotLight.GetBaseLightBuffer());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(3, 1, spotLight.GetSpotLightBuffer());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(7, 1, spotLight.GetViewProjBuffer());
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

