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
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~DrawerInterface();

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

protected:
	virtual void SetIAInputLayer() = 0;

protected:
	virtual void SetShader() = 0;

protected:
	virtual void SetOMState() = 0;

protected:
	virtual void ResetDrawer() = 0;
};

