#include "DrawerInterface.h"
#include "ModelInterface.h"
#include "CameraInterface.h"

DrawerInterface::DrawerInterface(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
}

DrawerInterface::~DrawerInterface()
{
}

void DrawerInterface::SetModel(ModelInterface* modelInterface)
{
	pModel = modelInterface;
}

void DrawerInterface::SetCamera(CameraInterface* cameraInterface)
{
	pCamera = cameraInterface;
}

void DrawerInterface::SetLightManager(LightManager* lightManager)
{
	pLightManager = lightManager;
}