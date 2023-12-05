#include "NonLightDrawer.h"
#include "ModelInterface.h"
#include "CameraInterface.h"

NonLightDrawer::NonLightDrawer(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: DrawerInterface(cpDeviceIn, cpDeviceContextIn)
{
}

NonLightDrawer::~NonLightDrawer()
{
}

void NonLightDrawer::Draw(CameraInterface* pCamera, ModelInterface* pModel)
{
	PresetConfig(pCamera);

	pModel->SetIAProperties();
	pModel->SetVSConstantBuffers();
	pModel->SetVSShaderResources();
	pModel->SetGSConstantBuffers();
	pModel->SetGSShaderResources();
	pModel->SetHSConstantBuffers();
	pModel->SetHSShaderResources();
	pModel->SetDSConstantBuffers();
	pModel->SetDSShaderResources();
	pModel->SetPSConstantBuffers();
	pModel->SetPSShaderResources();

	pModel->Render();

	pModel->ResetConstantBuffers();
	pModel->ResetShaderResources();

	ResetConfig(pCamera);
}

void NonLightDrawer::PresetConfig(
	CameraInterface* pCamera
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
}


void NonLightDrawer::ResetConfig(
	CameraInterface* pCamera
)
{
	pCamera->ResetCamera();

	ResetOMState();
	ResetDrawer();
}
