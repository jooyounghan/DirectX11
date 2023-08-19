#include "PortfolioApp.h"
#include "ICamera.h"
#include "IModel.h"

#include "TestModel.h"

#include <iostream>
#include <string>

#include <Windows.h>
#include <windowsx.h>

using namespace std;

PortfolioApp* gPortfolioApp = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gPortfolioApp->PortfolioProc(hWnd, msg, wParam, lParam);
}

PortfolioApp::PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn)
	: uiWidth(uiWidthIn),
	uiHeight(uiHeightIn),
	fAspectRatio(uiWidth / (float)uiHeight),
	imImgui(uiWidth, uiHeight),
	pMainCamera(nullptr)
{
	gPortfolioApp = this;

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

PortfolioApp::~PortfolioApp()
{

}

void PortfolioApp::Init()
{
	ID3D11Helper::CreateDeviceAndContext(uiWidth, uiHeight, true, hMainWindow, cpSwapChain.GetAddressOf(), cpDevice.GetAddressOf(), cpDeviceContext.GetAddressOf());
	imImgui.InitImgui(cpDevice.Get(), cpDeviceContext.Get(), hMainWindow);
	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, cpDeviceContext.Get(), &screenViewport);

	if (ICamera::DefaultCamera == nullptr)
	{
		ICamera::DefaultCamera = std::make_shared<ICamera>(cpDevice, cpDeviceContext, cpSwapChain, uiWidth, uiHeight);
	}
	pMainCamera = ICamera::DefaultCamera;
	vModels.push_back(std::make_shared<TestModel>(cpDevice, cpDeviceContext, 0.f, 0.f, 0.f, 2.f));
	vModels.push_back(std::make_shared<TestModel>(cpDevice, cpDeviceContext, 5.f, 0.f, 5.f, 2.f));
}

void PortfolioApp::Update()
{
	pMainCamera->Update();

	for (auto& model : vModels)
	{
		model->Update();
	}
}

void PortfolioApp::Render()
{
	pMainCamera->WipeOut();

	for (auto& model : vModels)
	{
		model->Render();
	}
}

void PortfolioApp::SwapChain()
{
	cpSwapChain->Present(1, 0);
}

void PortfolioApp::Run()
{
	// Main message loop
	MSG msg = {};
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Update();
			Render();
			SwapChain();
		}
	}
}

void PortfolioApp::Quit()
{
	DestroyWindow(hMainWindow);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void PortfolioApp::ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	if (cpSwapChain)
	{
		uiWidth = uiWidthIn;
		uiHeight = uiHeightIn;
		fAspectRatio = uiWidth / (float)uiHeight;
		cpSwapChain->ResizeBuffers(0, uiWidth, uiHeight, DXGI_FORMAT_UNKNOWN, 0);
		pMainCamera->Resize(fAspectRatio);
	}
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);


LRESULT __stdcall PortfolioApp::PortfolioProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	tagPOINT t;
	AutoZeroMemory(t);


	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg) {
	case WM_SIZE:
		ResizeSwapChain((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		return 0;
	case WM_MOUSEMOVE:
		pMainCamera->SetFromMouseXY(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
