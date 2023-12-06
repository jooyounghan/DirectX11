#pragma once

#include "DrawerInterface.h"

class LightlessDrawer : public DrawerInterface
{
public:
	LightlessDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~LightlessDrawer();

public:
	virtual void Draw(CameraInterface* pCamera, ModelInterface* pModel);

protected:
	virtual void SetIAInputLayer() = 0;

protected:
	virtual void SetShader() = 0;

protected:
	virtual void SetOMState() = 0;

protected:
	virtual void ResetDrawer() = 0;
};