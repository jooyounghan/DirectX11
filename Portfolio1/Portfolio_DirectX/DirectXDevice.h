#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

class DirectXDevice
{
public:
	static ID3D11Device*		pDevice;
	static ID3D11DeviceContext* pDeviceContext;
	static IDXGISwapChain*		pSwapChain;

private:
	static Microsoft::WRL::ComPtr<ID3D11Device> cpDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> cpDeviceContext;
	static Microsoft::WRL::ComPtr<IDXGISwapChain> cpSwapChain;

public:
	static void InitDevice(
		IN const UINT& uiWidthIn,
		IN const UINT& uiHeightIn,
		IN const DXGI_FORMAT& eFormatIn,
		IN const bool& bWindowed,
		IN HWND hOutputWindow
	);
};

