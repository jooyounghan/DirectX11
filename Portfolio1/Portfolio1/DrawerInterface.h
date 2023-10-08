#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

class ModelInterface;
class CameraInterface;
class LightManager;

class DrawerInterface
{
public:
	DrawerInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~DrawerInterface();

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

protected:
	ModelInterface* pModel;
	CameraInterface* pCamera;
	LightManager* pLightManager;

public:
	void SetModel(ModelInterface* modelInterface);
	void SetCamera(CameraInterface* cameraInterface);
	void SetLightManager(LightManager* lightManager);
};

