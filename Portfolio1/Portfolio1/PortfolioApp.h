#pragma once

#include "ImguiManager.h"
#include "IModel.h"
#include "ICamera.h"
class PortfolioApp
{
public:
	PortfolioApp(HWND hWindowIn, int& iWidthIn, int& iHeightIn);
	~PortfolioApp();

public:
	void Init();
	void Update() {};
	void Render();
	void SwapChain();

private:
	int& iWidth;
	int& iHeight;
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
	ICamera*		pMainCamera;
	vector<ICamera> vCameras;
	vector<IModel>	vModels;

private:
	ImguiManager imImgui;
};

