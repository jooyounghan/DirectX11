#include "PortfolioApp.h"
#include "DirectXDevice.h"

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
	InitImGUI();

	pCubeModel = new CubeModel(0.f, 0.f, 0.f, 2.f, false, 8);
	pPickableCamera = new PickableCamera(
		0.f, 0.f, -10.f, uiWidth, uiHeight,
		70.f * 2.f * 3.141592 / 360.f,
		0.01f, 1000.f, 0,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);

	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	ID3D11Helper::CreateVSInputLayOut(DirectXDevice::pDevice, L"VertexShader.hlsl", vInputElemDesc, cpVS.GetAddressOf(), cpIL.GetAddressOf());
	ID3D11Helper::CreatePS(DirectXDevice::pDevice, L"PixelShader.hlsl", cpPS.GetAddressOf());

	DirectXDevice::pDeviceContext->VSSetShader(cpVS.Get(), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(cpPS.Get(), NULL, NULL);

	//pPickableCamera->SetAsSwapChainBackBuffer();

	ComPtr<ID3D11Texture2D> backBuffer;
	DirectXDevice::pSwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	ID3D11Helper::CreateRenderTargetView(DirectXDevice::pDevice, backBuffer.Get(), backRTV.GetAddressOf());

	UINT uiStride = sizeof(InputLayout);
	UINT uiOffset = 0;
	DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, pCubeModel->inputBuffer.GetAddressOf(), &uiStride, &uiOffset);
	DirectXDevice::pDeviceContext->IASetIndexBuffer(pCubeModel->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	DirectXDevice::pDeviceContext->IASetInputLayout(cpIL.Get());
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &pPickableCamera->sViewPort);
	DirectXDevice::pDeviceContext->RSSetState(RasterizationState::GetInstance(DirectXDevice::pDevice, DirectXDevice::pDeviceContext).GetSolidRS());
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(DirectXDevice::pDevice);
	DirectXDevice::pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}

void PortfolioApp::Update(const float& fDelta)
{
	pCubeModel->Update(fDelta);
	pPickableCamera->Update(fDelta);
}

void PortfolioApp::Render()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, backRTV.GetAddressOf(), nullptr);
	DirectXDevice::pDeviceContext->ClearRenderTargetView(backRTV.Get(), clearColor);
	pPickableCamera->ClearRTV();
	pPickableCamera->ClearDSV();
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, pCubeModel->cpTransformationBuffer.GetAddressOf());
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pPickableCamera->cpViewProjBuffer.GetAddressOf());
	pCubeModel->Draw();
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

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
