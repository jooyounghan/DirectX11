#pragma once

#include <memory>

#include "ID3D11Helper.h"
#include "ImguiManager.h"

class ICamera;
class IModel;

class PortfolioApp
{
public:
	PortfolioApp(const UINT& iWidthIn, const UINT& iHeightIn);
	~PortfolioApp();

public:
	void Init();
	void Update();
	void Render();
	void SwapChain();
	void Run();
	void Quit();

public:
	void ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn);

private:
	UINT	uiWidth;
	UINT	uiHeight;
	float fAspectRatio;

private:
	WNDCLASSEX wc;
	HWND hMainWindow;

private:
	ComPtr<ID3D11Device> cpDevice;
	ComPtr<ID3D11DeviceContext> cpDeviceContext;
	ComPtr<IDXGISwapChain> cpSwapChain;

	ComPtr<ID3D11ShaderResourceView> cpShaderResourceView;
	ComPtr<ID3D11DepthStencilView> cpDepthStencilView;

	ComPtr<ID3D11RasterizerState> cpRasterizerState;
	ComPtr<ID3D11DepthStencilState> cpDepthStencilState;
	D3D11_VIEWPORT screenViewport;

private:	
	std::shared_ptr<ICamera>				pMainCamera;
	vector<std::shared_ptr<ICamera>>		vCameras;
	vector<std::shared_ptr<IModel>>			vModels;

private:
	ImguiManager imImgui;

public:
	LRESULT WINAPI PortfolioProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

