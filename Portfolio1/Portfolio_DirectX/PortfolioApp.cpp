#include "PortfolioApp.h"
#include "DirectXDevice.h"
#include "Shaders.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "ID3D11Helper.h"
#include "PickableCamera.h"
#include "CubeModel.h"
#include "SpotLight.h"
#include "PointLight.h"

using namespace std;

PortfolioApp::PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn)
	: BaseApp(uiWidthIn, uiHeightIn), pSelectedMesh(nullptr)
{
	BaseApp::GlobalBaseApp = this;

	mainSideBar.InitModelManipulator(&pSelectedMesh);
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
	AddModel(new CubeModel(-5.f, 0.f, 0.f, 1.f, false, 8));
	AddModel(new CubeModel(5.f, 0.f, 0.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, -5.f, 0.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, 5.f, 0.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, 0.f, 5.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, 0.f, -5.f, 1.f, false, 8));

	pMainCamera = new PickableCamera(
		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, uiWidth, uiHeight,
		70.f * 2.f * 3.141592f / 360.f,
		0.01f, 1000.f, 1,
		//DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);

	pLights.push_back(new SpotLight(0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
	pLights.push_back(new PointLight(0.f, 0.f, 0.f, 0.f, 0.f, 0.f));

	pMainCamera->SetAsBackBufferAddress();
}

void PortfolioApp::Update(const float& fDelta)
{
	for (auto model : pModels)
	{
		model.second->UpdateModel(fDelta);
	}

	pMainCamera->UpdatePosition();
	pMainCamera->UpdateView();
	pMainCamera->ManageKeyBoardInput(fDelta);

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
		pMainCamera->RenderTarget::cpRTV.Get(), 
		pMainCamera->IDPickableRenderTarget::cpRTV.Get() 
	};

	DirectXDevice::pDeviceContext->OMSetRenderTargets(2, vRTVs.data(), pMainCamera->cpDSV.Get());
	pMainCamera->ClearRTV();
	pMainCamera->ClearDSV();

	DirectXDevice::pDeviceContext->RSSetViewports(1, &pMainCamera->sViewPort);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, pMainCamera->cpViewProjBuffer.GetAddressOf());

	for (auto model : pModels)
	{
		DirectXDevice::pDeviceContext->IASetVertexBuffers(0, 1, model.second->cpInputBuffer.GetAddressOf(), &uiStride, &uiOffset);
		DirectXDevice::pDeviceContext->IASetIndexBuffer(model.second->cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, model.second->cpTransformationBuffer.GetAddressOf());
		DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, model.second->cpIdBuffer.GetAddressOf());

		model.second->Draw();
	}

	DirectXDevice::pDeviceContext->OMSetRenderTargets(2, vReleaseAndGetAddressOfRTVs.data(), nullptr);

	DirectXDevice::pDeviceContext->VSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->PSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->IASetInputLayout(shaders.GetInputLayout(Shaders::BaseVS));
	DirectXDevice::pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectXDevice::pDeviceContext->VSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->VSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->PSSetConstantBuffers(0, 1, &pNullBuffer);

	RenderImGUI();

	// ¸ðµ¨ ·»´õ¸µ ¹× ImGUI ·»´õ¸µ ÈÄ Resolve¸¦ ¼öÇàÇÑ´Ù.
	pMainCamera->Resolve();
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
	ImGui::StyleColorsClassic();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	ImGui_ImplDX11_Init(DirectXDevice::pDevice, DirectXDevice::pDeviceContext);
	ImGui_ImplWin32_Init(hMainWindow);
}

void PortfolioApp::SetImGUIRendering()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	mainSideBar.Draw();

	ImGui::Render();
}

void PortfolioApp::RenderImGUI()
{
	ID3D11RenderTargetView* pNullRTV = nullptr;
	DirectXDevice::pDeviceContext->OMSetRenderTargets(
		1, pMainCamera->RenderTarget::cpRTV.GetAddressOf(), pMainCamera->cpDSV.Get()
	);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	DirectXDevice::pDeviceContext->OMSetRenderTargets(
		1, &pNullRTV, nullptr
	);
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

	uint32_t uiSelectedID = 0;

	switch (msg) {
	case WM_EXITSIZEMOVE:
		pMainCamera->Resize(uiWidth, uiHeight);
		return 0;
	case WM_MOUSEMOVE:
		pMainCamera->ManageMouseInput(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYUP:
		switch (wParam) {
		case EKeyCode::W:
		case EKeyCode::A:
		case EKeyCode::S:
		case EKeyCode::D:
			pMainCamera->Release((EKeyCode)wParam);
			break;
		}
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case EKeyCode::W:
		case EKeyCode::A:
		case EKeyCode::S:
		case EKeyCode::D:
			pMainCamera->Press((EKeyCode)wParam);
			break;
		case EKeyCode::F:
			pMainCamera->Toggle((EKeyCode)wParam);
			break;
		}
		return 0;
	case WM_SIZE:
		uiWidth = (UINT)LOWORD(lParam);
		uiHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_LBUTTONDOWN:
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			pMainCamera->SetMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			uiSelectedID = pMainCamera->GetPickedID();
			pModels.find(uiSelectedID) != pModels.end() ? pSelectedMesh = pModels[uiSelectedID] : pSelectedMesh = nullptr;
		}
		return 0;
	case WM_LBUTTONUP:
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void PortfolioApp::AddModel(AStaticMesh* pModel)
{
	pModels.emplace(pModel->sModelData.uiModelID, pModel);
}
