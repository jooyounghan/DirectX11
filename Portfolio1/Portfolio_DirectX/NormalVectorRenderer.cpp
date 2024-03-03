#include "NormalVectorRenderer.h"

#include "Shaders.h"
#include "DirectXDevice.h"

#include "ACamera.h"
#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"

using namespace std;

NormalVectorRenderer::NormalVectorRenderer()
	: pCamera(nullptr)
{
}

NormalVectorRenderer::~NormalVectorRenderer()
{
}

void NormalVectorRenderer::RenderNormalVector(
	ACamera* pCameraIn, 
	const unordered_map<uint32_t, shared_ptr<IMesh>>& vMeshesIn
)
{
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
	
	auto [rtvCnt, vRTVs, pDSV] = pCameraIn->GetRTVs();
	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vRTVs.data(), pDSV);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &pCameraIn->sViewPort);

	Shaders& shaders = Shaders::GetInstance();

	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::NormalVectorVS), NULL, NULL);
	DirectXDevice::pDeviceContext->GSSetShader(shaders.GetGeometryShader(Shaders::NormalVectorGS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::NormalVectorPS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));

	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pCamera = pCameraIn;

	for (auto& staticMesh : vMeshesIn)
	{
		staticMesh.second->AcceptNormalVectorRendering(this);
	}

	pCamera = nullptr;

	vector<ID3D11RenderTargetView*> vNullRTVs;
	vNullRTVs.resize(rtvCnt, nullptr);
	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vNullRTVs.data(), nullptr);

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->GSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);

	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
}

void NormalVectorRenderer::RenderNormal(SinglePBRModel& singlePBRMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;
	
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, singlePBRMesh.GetTransformationBuffer());

	DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, singlePBRMesh.GetPBRConstantBuffer());
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, pCamera->GetViewProjBuffer());
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, singlePBRMesh.GetPBRTextureFlagBuffer());

	shared_ptr<IImageFile>& normalImage = singlePBRMesh.GetTextureImageFileRef(NORMAL_TEXTURE_MAP);
	shared_ptr<IImageFile>& heightImage = singlePBRMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

	DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1, 
		normalImage.get() != nullptr ?
		normalImage->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1,
		heightImage.get() != nullptr ?
		heightImage->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, DirectXDevice::ppClampSampler);

	singlePBRMesh.Draw();

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, &pNullSampler);
}

void NormalVectorRenderer::RenderNormal(GroupPBRModel& groupPBRMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	vector<PBRStaticMesh>& vPBRMeshes = groupPBRMesh.GetChildrenMeshesRef();

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, groupPBRMesh.GetTransformationBuffer());
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, pCamera->GetViewProjBuffer());
	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, DirectXDevice::ppClampSampler);

	for (PBRStaticMesh& pbrMesh : vPBRMeshes)
	{
		DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, pbrMesh.GetPBRConstantBuffer());
		DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, pbrMesh.GetPBRTextureFlagBuffer());

		shared_ptr<IImageFile>& normalImage = pbrMesh.GetTextureImageFileRef(NORMAL_TEXTURE_MAP);
		shared_ptr<IImageFile>& heightImage = pbrMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

		DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1,
			normalImage.get() != nullptr ?
			normalImage->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1,
			heightImage.get() != nullptr ?
			heightImage->GetAddressOfSRV() : &pNullSRV);

		pbrMesh.Draw();

		DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, &pNullBuffer);

		DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1, &pNullSRV);
		DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1, &pNullSRV);

	}
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, &pNullSampler);
}

void NormalVectorRenderer::RenderNormal(AIBLMesh& iblMesh)
{
	// Do Nothing
}
