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
#include "PickableCamera.h"
#include "CubeModel.h"
#include "SpotLight.h"
#include "PointLight.h"

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

	pModels.push_back(new CubeModel(-5.f, 0.f, 0.f, 1.f, false, 8));
	pModels.push_back(new CubeModel(5.f, 0.f, 0.f, 1.f, false, 8));
	pModels.push_back(new CubeModel(0.f, -5.f, 0.f, 1.f, false, 8));
	pModels.push_back(new CubeModel(0.f, 5.f, 0.f, 1.f, false, 8));
	pModels.push_back(new CubeModel(0.f, 0.f, 5.f, 1.f, false, 8));
	pModels.push_back(new CubeModel(0.f, 0.f, -5.f, 1.f, false, 8));

	pPickableCamera = new PickableCamera(
		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, uiWidth, uiHeight,
		70.f * 2.f * 3.141592f / 360.f,
		0.01f, 1000.f, 1,
		//DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);

	pLights.push_back(new SpotLight(0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
	pLights.push_back(new PointLight(0.f, 0.f, 0.f, 0.f, 0.f, 0.f));

	pPickableCamera->SetAsBackBufferAddress();
}

void PortfolioApp::Update(const float& fDelta)
{
	for (auto model : pModels)
	{
		model->UpdateModel(fDelta);
	}

	pPickableCamera->UpdatePosition();
	pPickableCamera->UpdateView();

	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
	for (auto& light : pLights)
	{
		light->UpdatePosition();
		light->UpdateLight(pModels);
	}
	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET);
}

void PortfolioApp::Render()
{
#pragma region ¸ðµ¨ ±×¸®±â
	Shaders& shaders = Shaders::GetInstance();
	DirectXDevice::pDeviceContext->VSSetShader(shaders.GetVertexShader(Shaders::BaseVS), NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(shaders.GetPixelShader(Shaders::BasePS), NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	vector<ID3D11RenderTargetView*> vReleaseAndGetAddressOfRTVs = {
	nullptr,
	nullptr
	};
	ID3D11Buffer* pNullBuffer = nullptr;

	UINT uiStride = sizeof(InputLayout);
	UINT uiOffset = 0;

	vector<ID3D11RenderTargetView*> vRTVs = { 
		pPickableCamera->RenderTarget::cpRTV.Get(), 
		pPickableCamera->IDPickableRenderTarget::cpRTV.Get() 
	};

	DirectXDevice::pDeviceContext->OMSetRenderTargets(2, vRTVs.data(), pPickableCamera->cpDSV.Get());
	pPickableCamera->ClearRTV();
	pPickableCamera->ClearDSV();

	DirectXDevice::pDeviceContext->RSSetViewports(1, &pPickableCamera->sViewPort);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pPickableCamera->cpViewProjBuffer.GetAddressOf());

	for (auto model : pModels)
	{
		DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, model->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
		DirectXDevice::pDeviceContext->IASetIndexBuffer(model->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, model->cpTransformationBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, model->cpIdBuffer.GetAddressOf());

		model->Draw();
	}

	DirectXDevice::pDeviceContext->OMSetRenderTargets(2, vReleaseAndGetAddressOfRTVs.data(), nullptr);

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);


#pragma endregion
	ID3D11RenderTargetView* pNullRTV = nullptr;
	DirectXDevice::pDeviceContext->OMSetRenderTargets(
		1, pPickableCamera->RenderTarget::cpRTV.GetAddressOf(), pPickableCamera->cpDSV.Get()
	);
	RenderImGUI();
	DirectXDevice::pDeviceContext->OMSetRenderTargets(
		1, &pNullRTV, nullptr
	);

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

	ImGui::Begin("Normal Setting");
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	ImGui::End();

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
	case WM_EXITSIZEMOVE:
		pPickableCamera->Resize(uiWidth, uiHeight);
		return 0;
	case WM_SIZE:
		uiWidth = (UINT)LOWORD(lParam);
		uiHeight = (UINT)HIWORD(lParam);
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
