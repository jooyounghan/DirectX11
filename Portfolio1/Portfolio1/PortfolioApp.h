#pragma once
#include <memory>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "BaseApp.h"

class ICamera;
class IModel;

class PortfolioApp : public BaseApp
{
public:
	PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn);
	~PortfolioApp();

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Run() override;
	virtual void Quit() override;

public:
	void InitImGUI();
	void SetImGUIRendering();
	void UpdateGUI();
	void RenderImGUI();
	void QuitImGUI();

public:
	void ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn);

private:
	ComPtr<ID3D11ShaderResourceView> cpShaderResourceView;
	ComPtr<ID3D11DepthStencilView> cpDepthStencilView;

	ComPtr<ID3D11RasterizerState> cpRasterizerState;
	ComPtr<ID3D11DepthStencilState> cpDepthStencilState;

private:	
	std::shared_ptr<ICamera>				pMainCamera;
	std::shared_ptr<IModel>					pSelectedModel;

private:
	vector<std::shared_ptr<ICamera>>		vCameras;
	vector<std::shared_ptr<IModel>>			vModels;

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

