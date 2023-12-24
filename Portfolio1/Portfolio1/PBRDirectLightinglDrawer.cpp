#include "PBRDirectLightingDrawer.h"

#include "CameraInterface.h"

#include "LightManager.h"
#include "LightInterface.h"

#include "PBRModel.h"

#include "DepthStencilState.h"
#include "SamplerState.h"
#include "RasterizationState.h"
#include "BlendState.h"

#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"

using namespace std;
using namespace Microsoft::WRL;

PBRDirectLightingDrawer::PBRDirectLightingDrawer(
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
	ID3D11Helper::CreatePS(pDevice, L"BaseModelDLPS.hlsl", cpBaseDLPS.GetAddressOf());

	ID3D11Helper::CreatePS(pDevice, L"OutlinerPS.hlsl", cpOutlinerPixelShader.GetAddressOf());
}

PBRDirectLightingDrawer::~PBRDirectLightingDrawer()
{
}

void PBRDirectLightingDrawer::SetIAInputLayer()
{
	pDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}

void PBRDirectLightingDrawer::SetShader()
{
	pDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
	pDeviceContext->HSSetShader(cpBaseHullShader.Get(), 0, 0);
	pDeviceContext->DSSetShader(cpBaseDomainShader.Get(), 0, 0);
	pDeviceContext->PSSetShader(cpBaseDLPS.Get(), 0, 0);

	SamplerState& samplerState = SamplerState::GetInstance(pDevice);

	pDeviceContext->DSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
	pDeviceContext->DSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, samplerState.GetAddressOfClampSampler());

	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
	pDeviceContext->PSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, samplerState.GetAddressOfClampSampler());
}

void PBRDirectLightingDrawer::SetOMState()
{
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}

void PBRDirectLightingDrawer::ResetDrawer()
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

	BlendState& blendState = BlendState::GetInstance(pDevice);
	ID3D11BlendState* pDefaultBS = blendState.pGetBS(BlendStateOption::DefaultBlendState);
	pDeviceContext->OMSetBlendState(pDefaultBS, BlendState::DefaultBlendFactor, 0xffffffff);

	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}


void PBRDirectLightingDrawer::Draw(
	CameraInterface* pCamera,
	LightManager* pLightManager,
	const std::vector<std::shared_ptr<PBRModel>> vSpModels
)
{
	BlendState& blendState = BlendState::GetInstance(pDevice);
	ID3D11BlendState* pAddBlendState = blendState.pGetBS(BlendStateOption::AddBlendState);

	RasterizationState& rasterizationState = RasterizationState::GetInstance(pDevice, pDeviceContext);

	pDeviceContext->RSSetState(rasterizationState.GetAppliedRS());
	pDeviceContext->OMSetBlendState(pAddBlendState, BlendState::DefaultBlendFactor, 0xfffffffff);

	SetIAInputLayer();
	SetShader();
	SetOMState();

	pCamera->SetConstantBuffers();
	pCamera->OMSetRenderTargets();

	const vector<LightInterface*>& pLights = pLightManager->GetLights();

	for (LightInterface* pLight : pLights)
	{
		pLight->SetConstantBuffers();

		for (auto& pObjectModel : vSpModels)
		{
			pObjectModel->SetIAProperties();
			pObjectModel->SetConstantBuffers();
			pObjectModel->SetShaderResources();

			pObjectModel->Render();

			pObjectModel->ResetConstantBuffers();
			pObjectModel->ResetShaderResources();
		}

		pLight->ResetConstantBuffers();
	}

	pCamera->ResetCamera();
	ResetDrawer();
}

