#pragma once

class ModelInterface;
class CameraInterface;
class LightManager;

class DrawerInterface
{
protected:
	ModelInterface* pModel;
	CameraInterface* pCamera;
	LightManager* pLightManager;

public:
	void SetModel(ModelInterface* modelInterface);
	void SetCamera(CameraInterface* cameraInterface);
	void SetLightManager(LightManager* lightManager);
};

