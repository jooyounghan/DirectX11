#include "APBRDirectLightDrawer.h"

#include "LightInterface.h"
#include "CameraInterface.h"
#include "PBRModel.h"

#include "DepthStencilState.h"
#include "SamplerState.h"
#include "RasterizationState.h"
#include "BlendState.h"

#include "ID3D11Helper.h"
#include "ShaderTypeEnum.h"

using namespace std;
using namespace Microsoft::WRL;

APBRDirectLightDrawer::APBRDirectLightDrawer(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: DrawerInterface(pDeviceIn, pDeviceContextIn)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	ID3D11Helper::CreateVSInputLayOut(pDeviceIn, L"BaseModelVS.hlsl", vInputElemDesc, cpBaseVertexShader.GetAddressOf(), cpBaseInputLayout.GetAddressOf());
	ID3D11Helper::CreateHS(pDeviceIn, L"BaseModelHS.hlsl", cpBaseHullShader.GetAddressOf());
	ID3D11Helper::CreateDS(pDeviceIn, L"BaseModelDS.hlsl", cpBaseDomainShader.GetAddressOf());
}

APBRDirectLightDrawer::~APBRDirectLightDrawer()
{
}

void APBRDirectLightDrawer::SetIAInputLayer()
{
	pDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}

void APBRDirectLightDrawer::SetShader()
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

void APBRDirectLightDrawer::SetOMState()
{
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}

void APBRDirectLightDrawer::ResetDrawer()
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
	pDeviceContext->OMSetBlendState(pDefaultBS, NULL, 0xffffffff);
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}

void APBRDirectLightDrawer::Draw(
	CameraInterface* pCamera,
	LightInterface* pLight,
	const std::vector<std::shared_ptr<PickableModelInterface>> vSpModels
)
{
	BlendState& blendState = BlendState::GetInstance(pDevice);
	ID3D11BlendState* pAddBlendState = blendState.pGetBS(BlendStateOption::AddBlendState);

	RasterizationState& rasterizationState = RasterizationState::GetInstance(pDevice, pDeviceContext);

	pDeviceContext->RSSetState(rasterizationState.GetAppliedRS());
	pDeviceContext->OMSetBlendState(pAddBlendState, NULL, UINT(0xfffffffff));

	SetIAInputLayer();
	SetShader();
	SetOMState();

	pCamera->SetConstantBuffers();
	pCamera->OMSetRenderTargets();

	pLight->SetConstantBuffers();
	pLight->SetShaderResources();

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
	pLight->ResetShaderResources();

	pCamera->ResetCamera();
	ResetDrawer();
}

