#include "ObjectDrawer.h"
#include "ObjectModel.h"
#include "CameraInterface.h"
#include "LightManager.h"
#include "CubeMapModel.h"

ObjectDrawer::ObjectDrawer(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: DrawerInterface(cpDeviceIn, cpDeviceContextIn)
{
}

ObjectDrawer::~ObjectDrawer()
{
}

void ObjectDrawer::Draw(
	CameraInterface* pCamera,
	LightManager* pLightManager,
	const std::vector<std::shared_ptr<PickableModel>> vSpModels,
	CubeMapModel* pEnvironmentCubeMap
)
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

	pLightManager->SetPSConstantBuffers();

	pEnvironmentCubeMap->SetPSConstantBuffers();
	pEnvironmentCubeMap->SetPSShaderResources();

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

		pObjectModel->Render();

		pObjectModel->ResetConstantBuffers();
		pObjectModel->ResetShaderResources();

	}

	pEnvironmentCubeMap->ResetConstantBuffers();
	pEnvironmentCubeMap->ResetShaderResources();

	pLightManager->ResetConstantBuffers();

	pCamera->ResetCamera();

	ResetOMState();
	ResetDrawer();
}
