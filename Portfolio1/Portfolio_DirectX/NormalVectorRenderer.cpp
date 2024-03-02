#include "NormalVectorRenderer.h"

#include "Shaders.h"
#include "DirectXDevice.h"

#include "ACamera.h"
#include "PBRStaticMesh.h"
#include "AIBLModel.h"

#include <vector>

NormalVectorRenderer::NormalVectorRenderer()
	: pCamera(nullptr)
{
}

NormalVectorRenderer::~NormalVectorRenderer()
{
}

void NormalVectorRenderer::RenderNormalVector(
	ACamera* pCameraIn, 
	const std::unordered_map<uint32_t, std::shared_ptr<AStaticMesh>>& vStaticMeshesIn
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

	for (auto& staticMesh : vStaticMeshesIn)
	{
		staticMesh.second->AcceptNormalVectorRendering(this);
	}

	pCamera = nullptr;

	std::vector<ID3D11RenderTargetView*> vNullRTVs;
	vNullRTVs.resize(rtvCnt, nullptr);
	DirectXDevice::pDeviceContext->OMSetRenderTargets(rtvCnt, vNullRTVs.data(), nullptr);

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->GSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(nullptr);

	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
}

void NormalVectorRenderer::RenderNormal(PBRStaticMesh& pbrStaticMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;
	
	auto [uiStrides, uiOffsets, vertexBuffers] = pbrStaticMesh.GetInputInfo();
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 4, vertexBuffers.data(), uiStrides.data(), uiOffsets.data());

	DirectXDevice::pDeviceContext->IASetIndexBuffer(pbrStaticMesh.spMeshFile->cpInicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pbrStaticMesh.cpTransformationBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, pbrStaticMesh.cpPBRConstantBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, pCamera->cpViewProjBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, pbrStaticMesh.cpPBRTextureFlagBuffer.GetAddressOf());

	DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1, 
		pbrStaticMesh.pModelTexture[NORMAL_TEXTURE_MAP] != nullptr ? 
		pbrStaticMesh.pModelTexture[NORMAL_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1,
		pbrStaticMesh.pModelTexture[HEIGHT_TEXTURE_MAP] != nullptr ?
		pbrStaticMesh.pModelTexture[HEIGHT_TEXTURE_MAP]->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, DirectXDevice::ppClampSampler);

	pbrStaticMesh.Draw();

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, &pNullSampler);

}

void NormalVectorRenderer::RenderNormal(AIBLModel& iblMesh)
{
}
