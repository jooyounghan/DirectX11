#include "LightlessDrawer.h"
#include "ModelInterface.h"
#include "CameraInterface.h"
#include "RasterizationState.h"

LightlessDrawer::LightlessDrawer(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: DrawerInterface(pDeviceIn, pDeviceContextIn)
{
}

LightlessDrawer::~LightlessDrawer()
{
}

void LightlessDrawer::Draw(CameraInterface* pCamera, ModelInterface* pModel)
{
	RasterizationState& rasterizationState = RasterizationState::GetInstance(pDevice, pDeviceContext);
	pDeviceContext->RSSetState(rasterizationState.GetAppliedRS());

	SetIAInputLayer();
	SetShader();
	SetOMState();

	pCamera->SetConstantBuffers();
	pCamera->OMSetRenderTargets();

	pModel->SetIAProperties();
	pModel->SetConstantBuffers();
	pModel->SetShaderResources();

	pModel->Render();

	pModel->ResetConstantBuffers();
	pModel->ResetShaderResources();

	pCamera->ResetCamera();

	ResetDrawer();
}
