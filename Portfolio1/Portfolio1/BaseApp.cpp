#include "BaseApp.h"
#include "ID3D11Helper.h"

BaseApp* BaseApp::GlobalBaseApp = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return BaseApp::GlobalBaseApp->AppProc(hWnd, msg, wParam, lParam);
}

BaseApp::BaseApp(const UINT& uiWidthIn, const UINT& uiHeightIn)
	: uiWidth(uiWidthIn),
	uiHeight(uiHeightIn),
	fAspectRatio(uiWidth / (float)uiHeight)
{
	GlobalBaseApp = this;

	AutoZeroMemory(wc);
	wc = { sizeof(WNDCLASSEX),
					 CS_CLASSDC,
					 WndProc,
					 0L,
					 0L,
					 GetModuleHandle(NULL),
					 NULL,
					 NULL,
					 NULL,
					 NULL,
					 L"Portfolio1", // lpszClassName, L-string
					 NULL };

	RegisterClassEx(&wc);

	RECT rWindowRect = { 0, 0, (LONG)uiWidth, (LONG)uiHeight };

	AdjustWindowRect(&rWindowRect, WS_OVERLAPPEDWINDOW, FALSE);

	const LONG& lTotalWidth = rWindowRect.right - rWindowRect.left;
	const LONG& lTotalHeight = rWindowRect.bottom - rWindowRect.top;

	HWND hWindow = CreateWindow(wc.lpszClassName, L"Joo YH / Portfolio1",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		lTotalWidth,
		lTotalHeight,
		NULL, NULL, wc.hInstance, NULL);

	hMainWindow = hWindow;

	ShowWindow(hWindow, SW_SHOWDEFAULT);
	UpdateWindow(hWindow);
}

BaseApp::~BaseApp()
{
}

void BaseApp::Init()
{
	ID3D11Helper::CreateDeviceAndContext(uiWidth, uiHeight, true, hMainWindow, uiNumLevelQuality, cpSwapChain, cpDevice, cpDeviceContext);
	ID3D11Helper::Init(cpDevice.Get(), cpDeviceContext.Get());
	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, cpDeviceContext.Get(), &sScreenViewport);
}

void BaseApp::SwapChain()
{
	cpSwapChain->Present(1, 0);
}

LRESULT __stdcall BaseApp::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void BaseApp::Quit()
{
	DestroyWindow(hMainWindow);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}
