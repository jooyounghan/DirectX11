#pragma once
#include <windowsx.h>
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>

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
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Run() = 0;
	virtual void Quit();

public:
	void SwapChain();

protected:
	Microsoft::WRL::ComPtr<ID3D11Device> cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> cpDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> cpSwapChain;

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

