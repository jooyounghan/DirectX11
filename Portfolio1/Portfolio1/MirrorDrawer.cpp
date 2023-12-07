#include "MirrorDrawer.h"
#include "ID3D11Helper.h"

#include "PBRModelDrawer.h"
#include "CubeMapDrawer.h"

#include "CameraInterface.h"
#include "LightManager.h"
#include "PBRModel.h"
#include "CubeMapModel.h"
#include "MirrorModel.h"


#include "SamplerState.h"
#include "DepthStencilState.h"
#include "ShaderTypeEnum.h"

#include <vector>

using namespace std;

MirrorDrawer::MirrorDrawer(
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

	ID3D11Helper::CreateVSInputLayOut(pDevice, L"BaseModelVS.hlsl", vInputElemDesc, cpMirrorVertexShader.GetAddressOf(), cpMirrorInputLayout.GetAddressOf());
	ID3D11Helper::CreatePS(pDevice, L"MirrorModelPS.hlsl", cpMirrorPixelShader.GetAddressOf());
}

MirrorDrawer::~MirrorDrawer()
{
}

void MirrorDrawer::Draw(
	CameraInterface* pCamera,
	LightManager* pLightManager,
	PBRModelDrawer* pPBRModelDrawer,
	const std::vector<std::shared_ptr<PBRModel>>& vSpModels,
	CubeMapDrawer* pCubeMapDrawer,
	CubeMapModel* pEnvironmentCubeMap,
	const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModels
)
{
	// Mirror의 ViewProj를 통하여 반사된 물체를 그린다.
	// 거울의 Render Target에 물체를 그린다.
	pPBRModelDrawer->SetIAInputLayer();
	pPBRModelDrawer->SetShader();
	pPBRModelDrawer->SetOMState();

	pLightManager->SetConstantBuffers();

	pEnvironmentCubeMap->SetConstantBuffers();
	pEnvironmentCubeMap->SetShaderResources();
	
	for (auto& pMirrorModel : vMirrorModels)
	{
		pMirrorModel->SetConstantBuffers();
		pMirrorModel->SetShaderResources();
		pMirrorModel->OMSetRenderTargets();

		for (auto& pObjectModel : vSpModels)
		{
			pObjectModel->SetIAProperties();
			pObjectModel->SetConstantBuffers();
			pObjectModel->SetShaderResources();

			pObjectModel->Render();

			pObjectModel->ResetConstantBuffers();
			pObjectModel->ResetShaderResources();
		}

		pMirrorModel->ResetConstantBuffers();
		pMirrorModel->ResetShaderResources();
		pMirrorModel->ResetCamera();
	}
	
	pEnvironmentCubeMap->ResetConstantBuffers();
	pEnvironmentCubeMap->ResetShaderResources();

	pLightManager->ResetConstantBuffers();

	pPBRModelDrawer->ResetDrawer();


	// Mirror의 ViewProj를 통하여 반사된 큐브맵를 그린다.
	// 거울의 Render Target에 물체를 그린다.

	pCubeMapDrawer->SetIAInputLayer();
	pCubeMapDrawer->SetShader();
	pCubeMapDrawer->SetOMState();

	for (auto& pMirrorModel : vMirrorModels)
	{
		pMirrorModel->SetConstantBuffers();
		pMirrorModel->SetShaderResources();
		pMirrorModel->OMSetRenderTargets();

		pEnvironmentCubeMap->SetIAProperties();
		pEnvironmentCubeMap->SetConstantBuffers();
		pEnvironmentCubeMap->SetShaderResources();

		pEnvironmentCubeMap->Render();

		pEnvironmentCubeMap->ResetConstantBuffers();
		pEnvironmentCubeMap->ResetShaderResources();

		pMirrorModel->ResetConstantBuffers();
		pMirrorModel->ResetShaderResources();
		pMirrorModel->ResetCamera();
	}
	pCubeMapDrawer->ResetDrawer();

	// 현재 거울을 그리는 방법에 대한 수정 필요
	// 거울을 렌더링 한다.
	SetIAInputLayer();
	SetShader();
	SetShader();
	SetOMState();
	for (auto& referenceMirror : vMirrorModels)
	{
		for (auto& renderedMirror : vMirrorModels)
		{
			renderedMirror->SetIAProperties();
			renderedMirror->SetConstantBuffers();
			renderedMirror->SetShaderResources();
			renderedMirror->OMSetRenderTargets();

			if (referenceMirror == renderedMirror)
			{
				pCamera->SetConstantBuffers();
				pCamera->OMSetRenderTargets();
			}
			renderedMirror->Render();

			pCamera->ResetCamera();
			renderedMirror->ResetConstantBuffers();
			renderedMirror->ResetShaderResources();
			renderedMirror->ResetCamera();
		}
	}
	ResetDrawer();
}

void MirrorDrawer::SetIAInputLayer()
{
	pDeviceContext->IASetInputLayout(cpMirrorInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MirrorDrawer::SetShader()
{
	pDeviceContext->VSSetShader(cpMirrorVertexShader.Get(), NULL, NULL);
	pDeviceContext->PSSetShader(cpMirrorPixelShader.Get(), NULL, NULL);

	SamplerState& samplerState = SamplerState::GetInstance(pDevice);
	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, samplerState.GetAddressOfWrapSampler());
	pDeviceContext->PSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, samplerState.GetAddressOfClampSampler());
}

void MirrorDrawer::SetOMState()
{
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}


void MirrorDrawer::ResetDrawer()
{
	pDeviceContext->PSSetShader(nullptr, 0, 0);
	pDeviceContext->VSSetShader(nullptr, 0, 0);

	ID3D11SamplerState* pResetSampler = nullptr;
	pDeviceContext->PSSetSamplers(SamplerType::WRAP_SAMPLER, 1, &pResetSampler);
	pDeviceContext->PSSetSamplers(SamplerType::CLAMP_SAMPLER, 1, &pResetSampler);
}

