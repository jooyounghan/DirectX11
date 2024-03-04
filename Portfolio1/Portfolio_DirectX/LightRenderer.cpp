#include "LightRenderer.h"

#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "DirectXDevice.h"
#include "Shaders.h"

using namespace std;

LightRenderer::LightRenderer()
	: IRenderer(), pModelSet(nullptr)
{
}

LightRenderer::~LightRenderer()
{
}

void LightRenderer::SetMessageFilter()
{
}

void LightRenderer::ResetMessageFilter()
{
}

void LightRenderer::UpdateLightMap(
	const unordered_map<uint32_t, shared_ptr<IMesh>>& vMeshes,
	const vector<shared_ptr<ILight>>& vLights
)
{
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();

	pModelSet = &vMeshes;
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

		Viewable& current_view = pointLight.GetViewable((EPointDirections)idx);
		DirectXDevice::pDeviceContext->RSSetViewports(1, &current_view.sViewPort);

		DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, current_view.GetViewProjBuffer());
		DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, pointLight.GetPositionBuffer());
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, current_view.GetViewProjBuffer());

		for (auto model : *pModelSet)
		{
			model.second->AcceptLightMapUpdateSetting(this);
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

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, spotLight.GetViewProjBuffer());
	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, spotLight.GetViewProjBuffer());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, spotLight.GetViewProjBuffer());


	for (auto model : *pModelSet)
	{
		model.second->AcceptLightMapUpdateSetting(this);
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

void LightRenderer::SetModelSettingForLightMap(SinglePBRModel& singlePBRMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, singlePBRMesh.GetTransformationBuffer());

	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, singlePBRMesh.GetPBRConstantBuffer());
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, singlePBRMesh.GetPBRTextureFlagBuffer());

	shared_ptr<IImageFile>& heightFile = singlePBRMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
		heightFile.get() != nullptr ?
		heightFile->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);

	singlePBRMesh.Draw();

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, &pNullSampler);
}

void LightRenderer::SetModelSettingForLightMap(GroupPBRModel& groupPBRMesh)
{
	ID3D11Buffer* pNullBuffer = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	vector<PBRStaticMesh> vPBRMeshes = groupPBRMesh.GetChildrenMeshesRef();

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, groupPBRMesh.GetTransformationBuffer());
	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);

	for (PBRStaticMesh& pbrMesh : vPBRMeshes)
	{
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, pbrMesh.GetPBRConstantBuffer());
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, pbrMesh.GetPBRTextureFlagBuffer());
		shared_ptr<IImageFile>& heightFile = pbrMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

		DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
			heightFile.get() != nullptr ?
			heightFile->GetAddressOfSRV() : &pNullSRV);

		pbrMesh.Draw();

		DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, &pNullBuffer);
		DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1, &pNullSRV);
	}
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, &pNullSampler);

}

void LightRenderer::SetModelSettingForLightMap(AIBLMesh& iblMesh)
{
	// Do Nothing
}

