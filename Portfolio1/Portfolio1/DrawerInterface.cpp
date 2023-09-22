#include "DrawerInterface.h"
#include "ModelInterface.h"
#include "CameraInterface.h"

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