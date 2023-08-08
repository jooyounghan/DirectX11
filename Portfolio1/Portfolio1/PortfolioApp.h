#pragma once

#include "ImguiManager.h"
#include "ID3D11Helper.h"

class PortfolioApp
{
public:
	PortfolioApp(HWND hWindowIn, const int& iWidthIn, const int& iHeightIn)
		: hMainWindow(hWindowIn), iWidth(iWidthIn), iHeight(iHeightIn), imImgui(iWidth, iHeight)
	{
		//ID3D11Helper::CreateDeviceAndContext(hWindowIn, iWidth, iHeight, true, cpSwapChain.GetAddressOf(), cpDevice.GetAddressOf(), cpDeviceContext.GetAddressOf());
		imImgui.InitImgui(cpDevice.Get(), cpDeviceContext.Get(), hMainWindow);
	};
	~PortfolioApp() {};

public:
	void Update() {};
	void Render() {};
	void SwapChain() {};

private:
	int iWidth;
	int iHeight;
	HWND hMainWindow;

private:
	ComPtr<ID3D11Device> cpDevice;
	ComPtr<ID3D11DeviceContext> cpDeviceContext;
	ComPtr<IDXGISwapChain> cpSwapChain;

	ComPtr<ID3D11InputLayout> cpInputLayout;
	ComPtr<ID3D11RasterizerState> cpRasterizerState;
	
	ComPtr<ID3D11ShaderResourceView> cpShaderResourceView;
	ComPtr<ID3D11DepthStencilView> cpDepthStencilView;
	ComPtr<ID3D11DepthStencilState> cpDepthStencilState;
	
	ComPtr<ID3D11VertexShader> cpVertexShader;
	ComPtr<ID3D11PixelShader> cpPixelShader;

	ImguiManager imImgui;
};

