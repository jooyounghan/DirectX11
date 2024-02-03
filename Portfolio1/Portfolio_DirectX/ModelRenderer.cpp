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
	pIBLModel = pIBLModelIn;
	vLights = &vLightsIn;

	for (auto meshes : vStaticMeshesIn)
	{
		meshes.second->AcceptModelRenderer(this);
	}

	pIBLModel = nullptr;
	vLights = nullptr;
}


void ModelRenderer::VisitModel(APBRStaticMesh& pbrStaticMesh)
{
#pragma region ¸ðµ¨ ±×¸®±â
#pragma region Preset
	UINT uiStride = sizeof(InputLayout);
	UINT uiOffset = 0;

	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::BasePS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, pbrStaticMesh.cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
	DirectXDevice::pDeviceContext->IASetIndexBuffer(pbrStaticMesh.cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &pCamera->sViewPort);

	auto [rtvCnt, vRTVs, pDSV] = pCamera->GetRTVs();

	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vRTVs.data(), pDSV);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pbrStaticMesh.cpTransformationBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pbrStaticMesh.cpIdBuffer.GetAddressOf());
#pragma endregion

	pbrStaticMesh.Draw();

#pragma region Reset
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);


	vector<ID3D11RenderTargetView*> vNullRTVs;
	vNullRTVs.resize(rtvCnt, nullptr);
	ID3D11Buffer* pNullBuffer = nullptr;

	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vNullRTVs.data(), nullptr);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
#pragma endregion
#pragma endregion
}

void ModelRenderer::VisitModel(AIBLModel& iblMesh)
{
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
	DirectXDevice::pDeviceContext->RSSetViewports(1, &pCamera->sViewPort);

	auto [rtvCnt, vRTVs, pDSV] = pCamera->GetRTVs();
	vector<ID3D11ShaderResourceView*> SRVs{
		iblMesh.spEnvSpecularTextureFile != nullptr ? iblMesh.spEnvSpecularTextureFile->cpSRV.Get() : nullptr,
		iblMesh.spEnvDiffuseTextureFile != nullptr ? iblMesh.spEnvDiffuseTextureFile->cpSRV.Get() : nullptr
	};

	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vRTVs.data(), pDSV);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, iblMesh.cpTransformationBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, iblMesh.cpIdBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetShaderResources(0, SRVs.size(), SRVs.data());
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, DirectXDevice::ppWrapSampler);
#pragma endregion

	iblMesh.Draw();

#pragma region Reset
	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);

	vector<ID3D11RenderTargetView*> vvNullRTVsRTVs;
	vvNullRTVsRTVs.resize(rtvCnt, nullptr);

	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	vector<ID3D11ShaderResourceView*> vNullSRVs;
	vNullSRVs.resize(SRVs.size(), nullptr);

	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vvNullRTVsRTVs.data(), nullptr);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetShaderResources(0, vNullSRVs.size(), vNullSRVs.data());
	DirectXDevice::pDeviceContext->PSSetSamplers(0, 1, &pNullSampler);
#pragma endregion
}
