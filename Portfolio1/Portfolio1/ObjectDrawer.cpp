#include "ObjectDrawer.h"
#include "ObjectModel.h"
#include "CameraInterface.h"
#include "LightManager.h"

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

void ObjectDrawer::Draw(CameraInterface* pCamera, LightManager* pLightManager, const std::vector<std::shared_ptr<ObjectModel>> vSpModels)
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

	pLightManager->ResetConstantBuffers();

	pCamera->ResetCamera();

	ResetOMState();
	ResetDrawer();
}
