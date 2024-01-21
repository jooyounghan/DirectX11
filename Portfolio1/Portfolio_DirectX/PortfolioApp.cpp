#include "PortfolioApp.h"
#include "DirectXDevice.h"
#include "Shaders.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

//Remove
#include "ID3D11Helper.h"
#include "RasterizationState.h"
#include "DepthStencilState.h"

using namespace std;

PortfolioApp::PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn)
	: BaseApp(uiWidthIn, uiHeightIn)
{
	BaseApp::GlobalBaseApp = this;
}

PortfolioApp::~PortfolioApp()
{
}

void PortfolioApp::Init()
{
	DirectXDevice::InitDevice(
		uiWidth, uiHeight, 
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		true, hMainWindow
	);

	Shaders& shaders = Shaders::GetInstance();
	shaders.Init(DirectXDevice::pDevice);

	InitImGUI();

	pCubeModel1 = new CubeModel(-2.f, 0.f, 0.f, 2.f, false, 8);
	pCubeModel2 = new CubeModel(2.f, 0.f, 0.f, 2.f, false, 8);
	pPickableCamera = new PickableCamera(
		0.f, 0.f, -10.f, uiWidth, uiHeight,
		70.f * 2.f * 3.141592f / 360.f,
		0.01f, 1000.f, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);

	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVertexShader), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::BasePixelShader), NULL, NULL);

	pPickableCamera->SetAsSwapChainBackBuffer();

	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVertexShader));
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &pPickableCamera->sViewPort);
	DirectXDevice::pDeviceContext->RSSetState(RasterizationState::GetInstance(DirectXDevice::pDevice, DirectXDevice::pDeviceContext).GetSolidRS());
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(DirectXDevice::pDevice);
	DirectXDevice::pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}

void PortfolioApp::Update(const float& fDelta)
{
	pCubeModel1->UpdateModel(fDelta);
	pCubeModel2->UpdateModel(fDelta);
	pPickableCamera->UpdateCamera(fDelta);
}

void PortfolioApp::Render()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	vector<ID3D11RenderTargetView*> vRTVs = { 
		pPickableCamera->ARenderTarget::cpRTV.Get(), 
		pPickableCamera->IDPickableRenderTarget::cpRTV.Get() 
	};
	vector<ID3D11RenderTargetView*> vResetRTVs = {
		nullptr,
		nullptr
	};

	DirectXDevice::pDeviceContext->OMSetRenderTargets(2, vRTVs.data(), nullptr);
	pPickableCamera->ClearRTV();
	pPickableCamera->ClearDSV();

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pPickableCamera->cpViewProjBuffer.GetAddressOf());

	UINT uiStride = sizeof(InputLayout);
	UINT uiOffset = 0;

	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, pCubeModel1->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
	DirectXDevice::pDeviceContext->IASetIndexBuffer(pCubeModel1->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pCubeModel1->cpTransformationBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pCubeModel1->cpIdBuffer.GetAddressOf());

	pCubeModel1->Draw();

	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, pCubeModel2->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
	DirectXDevice::pDeviceContext->IASetIndexBuffer(pCubeModel2->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pCubeModel2->cpTransformationBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, pCubeModel2->cpIdBuffer.GetAddressOf());

	pCubeModel2->Draw();

	DirectXDevice::pDeviceContext->OMSetRenderTargets(2, vResetRTVs.data(), nullptr);
	pPickableCamera->Resolve();
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
			SetImGUIRendering();
			Update(1.f / ImGui::GetIO().Framerate);
			Render();

			RenderImGUI();

			SwapChain();
		}
	}
}

void PortfolioApp::Quit()
{
	BaseApp::Quit();
	QuitImGUI();
}

void PortfolioApp::InitImGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.DisplaySize = ImVec2((float)uiWidth, (float)uiHeight);
	ImGui::StyleColorsDark();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	ImGui_ImplDX11_Init(DirectXDevice::pDevice, DirectXDevice::pDeviceContext);
	ImGui_ImplWin32_Init(hMainWindow);
}

void PortfolioApp::SetImGUIRendering()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Render();
}

void PortfolioApp::RenderImGUI()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void PortfolioApp::QuitImGUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

LRESULT __stdcall PortfolioApp::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg) {
	case WM_SIZE:
		pPickableCamera->Resize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		return 0;
	case WM_LBUTTONDOWN:
		pPickableCamera->SetMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
		Console::Print(std::to_string(pPickableCamera->GetPickedID()));
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
