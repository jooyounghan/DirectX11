#pragma once
#include "DrawerInterface.h"

#include <memory>
#include <vector>

class PickableModel;
class CameraInterface;
class LightManager;
class CubeMapModel;

class ObjectDrawer : public DrawerInterface
{
public:
	ObjectDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~ObjectDrawer();

public:
	void Draw(
		CameraInterface* pCamera, 
		LightManager* pLightManager, 
		const std::vector<std::shared_ptr<PickableModel>> vSpModels,
		CubeMapModel* pEnvironmentCubeMap
	);

protected:
	virtual void SetIAInputLayer() = 0;
	virtual void SetVSShader() = 0;
	virtual void SetHSShader() = 0;
	virtual void SetDSShader() = 0;
	virtual void SetGSShader() = 0;
	virtual void SetPSShader() = 0;

protected:
	virtual void SetOMState() = 0;
	virtual void ResetOMState() = 0;

protected:
	virtual void ResetDrawer() = 0;
};

