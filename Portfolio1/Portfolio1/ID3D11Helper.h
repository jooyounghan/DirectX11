#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h> 

using Microsoft::WRL::ComPtr;

class ID3D11Helper
{
public:
	static void CreateDeviceAndContext(HWND hOutputWindow, const int& iWidth, const int& iHeight, bool bWindowed, IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext);
};

