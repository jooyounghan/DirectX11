#include "PortfolioApp.h"
#include "DirectXDevice.h"
#include "Shaders.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "IMesh.h"

#include "ID3D11Helper.h"
#include "ACamera.h"
#include "ILight.h"

using namespace std;

PortfolioApp::PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn)
	: 
	BaseApp(uiWidthIn, uiHeightIn)
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

	upModelManipulator = make_unique<ModelManipulator>();
	upFileManipulator = make_unique<FileManipulator>();
	upStageManipulator = make_unique<StageManipulator>(uiWidth, uiHeight, upModelManipulator.get());

	upModelRenderer = make_unique<ModelRenderer>();
	upLightRenderer = make_unique<LightRenderer>();
	upNormalVectorRenderer = make_unique<NormalVectorRenderer>();

	InitImGUI();
}

void PortfolioApp::Update(const float& fDelta)
{
	for (const auto& model : upModelManipulator->GetModels())
	{
		model.second->UpdateModel(fDelta);
	}

	CameraManipulator* const pCameraManipulator = upStageManipulator->GetCameraManipulator();
	LightManipulator* const pLightManipulator = upStageManipulator->GetLightManipulator();

	ACamera* pCamera = pCameraManipulator->GetSelectedCamera();
	if (pCamera)
	{
		pCamera->UpdatePosition();
		pCamera->UpdateView();
		pCamera->ManageKeyBoardInput(fDelta);
	}

	for (auto& light : pLightManipulator->GetLights())
	{
		light->UpdatePosition();
		light->UpdateLight();
	}
}

void PortfolioApp::Render()
{
	CameraManipulator* const pCameraManipulator = upStageManipulator->GetCameraManipulator();
	LightManipulator* const pLightManipulator = upStageManipulator->GetLightManipulator();

	ACamera* pCamera = pCameraManipulator->GetSelectedCamera();
	if (pCamera)
	{
		pCamera->ClearRTV();
		pCamera->ClearDSV();

		const std::vector<std::shared_ptr<ILight>>& pLights = pLightManipulator->GetLights();

		const shared_ptr<AIBLMesh>& spIBLModel = upModelManipulator->GetIBLModel();
		const unordered_map<uint32_t, std::shared_ptr<IMesh>>& pModels = upModelManipulator->GetModels();

		//upLightRenderer->UpdateLightMap(pModels, pLights);

		if (upModelManipulator->GetIsDrawingNormal())
		{
			upNormalVectorRenderer->RenderNormalVector(pCamera, pModels);
		}

		//upModelRenderer->RenderObjects(pCamera, spIBLModel, pModels, pLights);

		pCamera->Resolve();
	}
	else
	{
		if (DirectXDevice::pRenderTargetView != nullptr)
		{
			DirectXDevice::pDeviceContext->ClearRenderTargetView(
				DirectXDevice::pRenderTargetView,
				RenderTarget::fClearColor
			);
		}
	}

	RenderImGUI();
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

	upModelManipulator->PopAsDialog();
	upFileManipulator->PopAsDialog();
	upStageManipulator->PopAsDialog();

	ImGui::Render();
}

void PortfolioApp::RenderImGUI()
{
	ID3D11RenderTargetView* pNullRTV = nullptr;
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &DirectXDevice::pRenderTargetView, nullptr);

	ImDrawData* test = ImGui::GetDrawData();
	ImGui_ImplDX11_RenderDrawData(test);

	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);
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

	CameraManipulator* const pCameraManipulator = upStageManipulator->GetCameraManipulator();
	LightManipulator* const pLightManipulator = upStageManipulator->GetLightManipulator();

	pCameraManipulator->ProcWindowMsg(msg, wParam, lParam);

	switch (msg) {
	case WM_EXITSIZEMOVE:
		pCameraManipulator->ResizeSelectedCamera(uiWidth, uiHeight);
		return 0;
	case WM_SIZE:
		uiWidth = (UINT)LOWORD(lParam);
		uiHeight = (UINT)HIWORD(lParam);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
