#pragma once
#include <windowsx.h>
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "DirectXDevice.h"

class BaseApp
{
public:
	BaseApp(const UINT& uiWidthIn, const UINT& uiHeightIn);
	~BaseApp();

public:
	static BaseApp* GlobalBaseApp;

protected:
	UINT	uiWidth;
	UINT	uiHeight;

protected:
	WNDCLASSEX wc;
	HWND hMainWindow;

public:
	virtual void Init() = 0;
	virtual void Update(const float& fDelta) = 0;
	virtual void Render() = 0;
	virtual void Run() = 0;
	virtual void Quit();

public:
	inline void SwapChain() { DirectXDevice::pSwapChain->Present(1, 0); };

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

