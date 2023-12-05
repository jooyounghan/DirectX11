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
	virtual ~DrawerInterface();

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

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
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	cpDrawerWrapSampler;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	cpDrawerClampSampler;
};

