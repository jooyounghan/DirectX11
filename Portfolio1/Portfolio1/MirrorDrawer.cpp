#include "MirrorDrawer.h"
#include "ID3D11Helper.h"

#include "ObjectDrawer.h"
#include "CubeMapDrawer.h"
#include "CubeMapModel.h"

#include "CameraInterface.h"
#include "LightManager.h"
#include "ObjectModel.h"
#include "MirrorModel.h"
#include "ShaderTypeEnum.h"
#include "DepthStencilState.h"

#include <vector>

using namespace std;

MirrorDrawer::MirrorDrawer(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: DrawerInterface(cpDeviceIn, cpDeviceContextIn)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(cpDevice.Get(), L"BaseModelVS.hlsl", vInputElemDesc, cpMirrorVertexShader.GetAddressOf(), cpMirrorInputLayout.GetAddressOf());
	ID3D11Helper::CreatePS(cpDevice.Get(), L"MirrorModelPS.hlsl", cpMirrorPixelShader.GetAddressOf());
}

MirrorDrawer::~MirrorDrawer()
{
}

void MirrorDrawer::Draw(
	class ObjectDrawer* pModelDrawer,
	class CubeMapDrawer* pCubeMapDrawer,
	CameraInterface* pCamera,
	LightManager* pLightManager,
	const std::vector<std::shared_ptr<ObjectModel>>& vSpModels,
	CubeMapModel* pEnvironmentCubeMap,
	const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModels
)
{
	// Mirror 평면의 Normal 방향에 대한 ViewProj를 통하여 물체를 그린다.
	// 거울의 Render Target에 물체를 그린다.
	pModelDrawer->PresetConfig(pCamera, pLightManager, pEnvironmentCubeMap);
	for (auto& pMirrorModel : vMirrorModels)
	{
		for (auto& pObjectModel : vSpModels)
		{
			pObjectModel->SetIAProperties();
			pObjectModel->SetVSConstantBuffers();
			pObjectModel->SetVSShaderResources();
			pObjectModel->SetGSConstantBuffers();
			pObjectModel->SetGSShaderResources();
			pObjectModel->SetHSConstantBuffers();
			pObjectModel->SetHSShaderResources();
			pObjectModel->SetDSConstantBuffers();
			pObjectModel->SetDSShaderResources();
			pObjectModel->SetPSConstantBuffers();
			pObjectModel->SetPSShaderResources();

			pMirrorModel->SetMirrorConstantBuffer();
			pMirrorModel->SetOMMirrorRenderTarget();

			pObjectModel->Render();

			pObjectModel->ResetConstantBuffers();
			pObjectModel->ResetShaderResources();
		}
	}
	pModelDrawer->ResetConfig(pCamera, pLightManager, pEnvironmentCubeMap);

	pCubeMapDrawer->PresetConfig(pCamera);
	for (auto& pMirrorModel : vMirrorModels)
	{
		pEnvironmentCubeMap->SetIAProperties();
		pEnvironmentCubeMap->SetVSConstantBuffers();
		pEnvironmentCubeMap->SetVSShaderResources();
		pEnvironmentCubeMap->SetGSConstantBuffers();
		pEnvironmentCubeMap->SetGSShaderResources();
		pEnvironmentCubeMap->SetHSConstantBuffers();
		pEnvironmentCubeMap->SetHSShaderResources();
		pEnvironmentCubeMap->SetDSConstantBuffers();
		pEnvironmentCubeMap->SetDSShaderResources();
		pEnvironmentCubeMap->SetPSConstantBuffers();
		pEnvironmentCubeMap->SetPSShaderResources();

		pMirrorModel->SetMirrorConstantBuffer();
		pMirrorModel->SetOMMirrorRenderTarget();

		pEnvironmentCubeMap->Render();

		pMirrorModel->ResetOMMirrorRenderTarget();
		pMirrorModel->ResetMirrorConstantBuffer();

		pEnvironmentCubeMap->ResetConstantBuffers();
		pEnvironmentCubeMap->ResetShaderResources();
	}
	pCubeMapDrawer->ResetConfig(pCamera);

	// 반사된 거울을 렌더링 한다.

	for (auto& referenceMirror : vMirrorModels)
	{

		for (auto& renderedMirror : vMirrorModels)
		{
			PresetConfig(pCamera);
				renderedMirror->SetIAProperties();
				renderedMirror->SetVSConstantBuffers();
				renderedMirror->SetVSShaderResources();
				renderedMirror->SetGSConstantBuffers();
				renderedMirror->SetGSShaderResources();
				renderedMirror->SetHSConstantBuffers();
				renderedMirror->SetHSShaderResources();
				renderedMirror->SetDSConstantBuffers();
				renderedMirror->SetDSShaderResources();
				renderedMirror->SetPSConstantBuffers();
				renderedMirror->SetPSShaderResources();

				if (referenceMirror != renderedMirror)
				{
					referenceMirror->SetMirrorConstantBuffer();
					referenceMirror->SetOMMirrorRenderTarget();

					renderedMirror->Render();

					referenceMirror->ResetOMMirrorRenderTarget();
					referenceMirror->ResetMirrorConstantBuffer();

				}
				else
				{
					renderedMirror->Render();
				}

				renderedMirror->ResetConstantBuffers();
				renderedMirror->ResetShaderResources();
				ResetConfig(pCamera);
		}
	}

}

void MirrorDrawer::SetIAInputLayer()
{
	cpDeviceContext->IASetInputLayout(cpMirrorInputLayout.Get());
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MirrorDrawer::SetVSShader()
{
	cpDeviceContext->VSSetShader(cpMirrorVertexShader.Get(), NULL, NULL);
}

void MirrorDrawer::SetHSShader()
{
}

void MirrorDrawer::SetDSShader()
{
}

void MirrorDrawer::SetGSShader()
{
}

void MirrorDrawer::SetPSShader()
{
	cpDeviceContext->PSSetShader(cpMirrorPixelShader.Get(), NULL, NULL);
	cpDeviceContext->PSSetSamplers(PSSamplerType::PS_WRAP_SAMPLER, 1, cpDrawerWrapSampler.GetAddressOf());
	cpDeviceContext->PSSetSamplers(PSSamplerType::PS_CLAMP_SAMPLER, 1, cpDrawerClampSampler.GetAddressOf());
}

void MirrorDrawer::SetOMState()
{
	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DefaultOption), 0);
}

void MirrorDrawer::ResetOMState()
{
	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DefaultOption), 0);
}

void MirrorDrawer::ResetDrawer()
{
	cpDeviceContext->PSSetShader(nullptr, 0, 0);
	cpDeviceContext->VSSetShader(nullptr, 0, 0);

	ID3D11SamplerState* pResetSampler = nullptr;
	cpDeviceContext->PSSetSamplers(PSSamplerType::PS_WRAP_SAMPLER, 1, &pResetSampler);
	cpDeviceContext->PSSetSamplers(PSSamplerType::PS_CLAMP_SAMPLER, 1, &pResetSampler);
}

void MirrorDrawer::PresetConfig(CameraInterface* pCamera)
{
	SetIAInputLayer();
	SetVSShader();
	SetGSShader();
	SetHSShader();
	SetDSShader();
	SetPSShader();
	SetOMState();

	pCamera->SetRSState();
	pCamera->SetVSConstantBuffers();
	pCamera->SetGSConstantBuffers();
	pCamera->SetHSConstantBuffers();
	pCamera->SetDSConstantBuffers();
	pCamera->SetPSConstantBuffers();
	pCamera->OMSetRenderTargets();
}

void MirrorDrawer::ResetConfig(CameraInterface* pCamera)
{
	pCamera->ResetCamera();

	ResetOMState();
	ResetDrawer();
}

