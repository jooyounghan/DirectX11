#pragma once
#include "ID3D11Helper.h"
#include <windowsx.h>

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
	float	fAspectRatio;
	UINT	uiNumLevelQuality;

protected:
	WNDCLASSEX wc;
	HWND hMainWindow;

public:
	virtual void Init();
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Run() = 0;
	virtual void Quit();

public:
	void SwapChain();

protected:
	ComPtr<ID3D11Device> cpDevice;
	ComPtr<ID3D11DeviceContext> cpDeviceContext;
	ComPtr<IDXGISwapChain> cpSwapChain;
	D3D11_VIEWPORT screenViewport;

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

