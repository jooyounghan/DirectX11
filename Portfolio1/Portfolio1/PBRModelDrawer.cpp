#include "PBRModelDrawer.h"

#include "CameraInterface.h"
#include "LightManager.h"
#include "PBRModel.h"
#include "CubeMapModel.h"

#include "DepthStencilState.h"
#include "SamplerState.h"
#include "RasterizationState.h"

#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"

using namespace std;
using namespace Microsoft::WRL;

PBRModelDrawer::PBRModelDrawer(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: DrawerInterface(pDeviceIn, pDeviceContextIn)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(pDeviceIn, L"BaseModelVS.hlsl", vInputElemDesc, cpBaseVertexShader.GetAddressOf(), cpBaseInputLayout.GetAddressOf());
	ID3D11Helper::CreateHS(pDeviceIn, L"BaseModelHS.hlsl", cpBaseHullShader.GetAddressOf());
	ID3D11Helper::CreateDS(pDeviceIn, L"BaseModelDS.hlsl", cpBaseDomainShader.GetAddressOf());
	ID3D11Helper::CreatePS(pDeviceIn, L"BaseModelPS.hlsl", cpBasePixelShader.GetAddressOf());
}

PBRModelDrawer::~PBRModelDrawer()
{
}

void PBRModelDrawer::SetIAInputLayer()
{
	pDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}

void PBRModelDrawer::SetShader()
{
	pDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
	pDeviceContext->HSSetShader(cpBaseHullShader.Get(), 0, 0);
	pDeviceContext->DSSetShader(cpBaseDomainShader.Get(), 0, 0);
	pDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);

	SamplerState& samplerState = SamplerState::GetInstance(pDevice);

	pDeviceContext->DSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
	pDeviceContext->DSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, samplerState.GetAddressOfClampSampler());

	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
	pDeviceContext->PSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, samplerState.GetAddressOfClampSampler());
}

void PBRModelDrawer::SetOMState()
{
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::MaskOption), 1);
}

void PBRModelDrawer::ResetDrawer()
{
	pDeviceContext->PSSetShader(nullptr, 0, 0);
	pDeviceContext->DSSetShader(nullptr, 0, 0);
	pDeviceContext->HSSetShader(nullptr, 0, 0);
	pDeviceContext->VSSetShader(nullptr, 0, 0);

	ID3D11SamplerState* pResetSampler = nullptr;
	pDeviceContext->DSSetSamplers(0, SamplerType::WRAP_SAMPLER, &pResetSampler);
	pDeviceContext->DSSetSamplers(0, SamplerType::CLAMP_SAMPLER, &pResetSampler);

	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, &pResetSampler);
	pDeviceContext->PSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, &pResetSampler);

	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}


void PBRModelDrawer::Draw(
	CameraInterface* pCamera,
	LightManager* pLightManager,
	const std::vector<std::shared_ptr<PBRModel>> vSpModels,
	CubeMapModel* pEnvironmentCubeMap
)
{
	RasterizationState& rasterizationState = RasterizationState::GetInstance(pDevice, pDeviceContext);
	pDeviceContext->RSSetState(rasterizationState.GetAppliedRS());

	SetIAInputLayer();
	SetShader();
	SetOMState();

	pCamera->SetConstantBuffers();
	pCamera->OMSetRenderTargets();

	pLightManager->SetConstantBuffers();

	pEnvironmentCubeMap->SetConstantBuffers();
	pEnvironmentCubeMap->SetShaderResources();

	for (auto& pObjectModel : vSpModels)
	{
		pObjectModel->SetIAProperties();
		pObjectModel->SetConstantBuffers();
		pObjectModel->SetShaderResources();

		pObjectModel->Render();

		pObjectModel->ResetConstantBuffers();
		pObjectModel->ResetShaderResources();
	}

	pEnvironmentCubeMap->ResetConstantBuffers();
	pEnvironmentCubeMap->ResetShaderResources();

	pLightManager->ResetConstantBuffers();

	pCamera->ResetCamera();
	ResetDrawer();
}

