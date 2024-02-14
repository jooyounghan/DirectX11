#include "PortfolioApp.h"
#include "DirectXDevice.h"
#include "Shaders.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "ModelRenderer.h"

#include "ID3D11Helper.h"
#include "PickableCamera.h"
#include "CubeModel.h"
#include "CubeMapModel.h"
#include "SpotLight.h"
#include "PointLight.h"

using namespace std;

PortfolioApp::PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn)
	: 
	BaseApp(uiWidthIn, uiHeightIn), 
	stageManipulator(uiWidth, uiHeight, pLights, pCameras)
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
	AddModel(new CubeModel(-5.f, 0.f, 0.f, 1.f, false, 8));
	AddModel(new CubeModel(5.f, 0.f, 0.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, -5.f, 0.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, 5.f, 0.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, 0.f, 5.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, 0.f, -5.f, 1.f, false, 8));
	AddModel(new CubeModel(0.f, 0.f, -5.f, 1.f, false, 8));

	pIBLModel = new CubeMapModel(500.f, 15);
	AddModel(pIBLModel);
}

void PortfolioApp::Update(const float& fDelta)
{
	for (auto model : pModels)
	{
		model.second->UpdateModel(fDelta);
	}

	ACamera* pCamera = stageManipulator.GetSelectedCamera();
	if (pCamera)
	{
		pCamera->UpdatePosition();
		pCamera->UpdateView();
		pCamera->ManageKeyBoardInput(fDelta);
	}

	for (auto& light : pLights)
	{
		light->UpdatePosition();
		light->UpdateLight();
	}
}

void PortfolioApp::Render()
{
	ACamera* pCamera = stageManipulator.GetSelectedCamera();
	if (pCamera)
	{
		pCamera->ClearRTV();
		pCamera->ClearDSV();

		lightRenderer.UpdateLightMap(pModels, pLights);

		if (modelManipulator.GetIsDrawingNormal())
		{
			normalVectorRenderer.RenderNormalVector(pCamera, pModels);
		}

		modelRenderer.RenderObjects(pCamera, pIBLModel, pModels, pLights);

		pCamera->Resolve();
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

	modelManipulator.PopAsDialog();
	fileManipulator.PopAsDialog();
	stageManipulator.PopAsDialog();

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

	uint32_t uiSelectedID = 0;

	ACamera* pCamera = stageManipulator.GetSelectedCamera();
	PickableCamera* pPickableCamera = dynamic_cast<PickableCamera*>(pCamera);

	if (pCamera)
	{
		switch (msg) {
		case WM_EXITSIZEMOVE:
			pCamera->Resize(uiWidth, uiHeight);
			return 0;
		case WM_MOUSEMOVE:
			pCamera->ManageMouseInput(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_KEYUP:
			switch (wParam)
			{
			case EKeyCode::W:
			case EKeyCode::A:
			case EKeyCode::S:
			case EKeyCode::D:
				pCamera->Release((EKeyCode)wParam);
				break;
			}
			return 0;
		case WM_KEYDOWN:
			switch (wParam) {
			case EKeyCode::W:
			case EKeyCode::A:
			case EKeyCode::S:
			case EKeyCode::D:
				pCamera->Press((EKeyCode)wParam);
				break;
			case EKeyCode::F:
				pCamera->Toggle((EKeyCode)wParam);
				break;
			}
			return 0;
		case WM_SIZE:
			uiWidth = (UINT)LOWORD(lParam);
			uiHeight = (UINT)HIWORD(lParam);
			return 0;
		case WM_LBUTTONDOWN:
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && pPickableCamera != nullptr)
			{
				pPickableCamera->SetMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				uiSelectedID = pPickableCamera->GetPickedID();
				pModels.find(uiSelectedID) != pModels.end() ?
					modelManipulator.SetAddressOfSelectedMesh(&pModels[uiSelectedID])
					: modelManipulator.SetAddressOfSelectedMesh(nullptr);
			}
			return 0;
		case WM_LBUTTONUP:
			return 0;
		}
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void PortfolioApp::AddModel(AStaticMesh* pModel)
{
	pModels.emplace(pModel->sModelData.uiModelID, pModel);
}
