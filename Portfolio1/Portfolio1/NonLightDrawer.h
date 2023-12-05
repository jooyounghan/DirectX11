#pragma once

#include "DrawerInterface.h"

class NonLightDrawer : public DrawerInterface
{
public:
	NonLightDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~NonLightDrawer();

public:
	virtual void Draw(CameraInterface* pCamera, ModelInterface* pModel);

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

protected:
	void PresetConfig(
		CameraInterface* pCamera
	);

	void ResetConfig(
		CameraInterface* pCamera
	);
};