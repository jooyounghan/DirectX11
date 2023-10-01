#include "PortfolioApp.h"
#include "ID3D11Helper.h"

#include "ModelManageGui.h"
#include "LightManageGui.h"
#include "FileManageGui.h"

#include "BaseModelDrawer.h"
#include "ModelOutlineDrawer.h"

#include "Canvas.h"

#include "CameraInterface.h"
#include "ModelInterface.h"

#include "LightManager.h"
#include "FileManager.h"


#include "DefineVar.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

using namespace DirectX;

using namespace std;

PortfolioApp::PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn)
	: BaseApp(uiWidthIn, uiHeightIn), spMainCamera(nullptr), spSelectedModel(nullptr), spTempSelectedModel(nullptr)
{
	BaseApp::GlobalBaseApp = this;
}

PortfolioApp::~PortfolioApp()
{

}

void PortfolioApp::Init()
{
	ID3D11Helper::CreateDeviceAndContext(uiWidth, uiHeight, true, hMainWindow, uiNumLevelQuality, cpSwapChain, cpDevice, cpDeviceContext);
	ID3D11Helper::Init(cpDevice.Get(), cpDeviceContext.Get());
	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, cpDeviceContext.Get(), &sScreenViewport);

	InitImGUI();

	// GUI 추가 =====================================================================================
	upModelManageGui = make_unique<ModelManageGui>(vSpModels, spSelectedModel, spTempSelectedModel);
	upLightManageGui = make_unique<LightManageGui>(spLightManager);
	upFileManageGui = make_unique<FileManageGui>(upFileManager);
	// ==============================================================================================

	upFileManager = make_unique<FileManager>(cpDevice, cpDeviceContext);
	spLightManager = make_unique<LightManager>(cpDevice, cpDeviceContext);
	upModelDrawer = make_unique<BaseModelDrawer>(cpDevice, cpDeviceContext);
	upModelOutlineDrawer = make_unique<ModelOutlineDrawer>(cpDevice, cpDeviceContext);

	// For Testing ==================================================================================
	upFileManager->LoadImageFromFile(L".\\Texture\\GrassWithMudAndStone");

	spMainCamera = make_shared<CameraInterface>(cpDevice, cpDeviceContext, cpSwapChain, uiWidth, uiHeight, uiNumLevelQuality);
	vSpModels.push_back(std::make_shared<ModelInterface>(cpDevice, cpDeviceContext, 0.f, 0.f, 0.f, 2.f));
	vSpModels.push_back(std::make_shared<ModelInterface>(cpDevice, cpDeviceContext, 5.f, 0.f, 5.f, 2.f));

	spLightManager->AddDirectionalLight(
		XMVectorSet(0.f, 0.f, -100.f, 1.f),
		XMVectorSet(1.f, 0.1f, 0.1f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f)
	);
	// ==============================================================================================
}

void PortfolioApp::Update()
{
	spMainCamera->Update();
	spLightManager->Update();

	CheckMouseHoveredModel();

	for (auto& model : vSpModels)
	{
		model->Update();
	}
}

void PortfolioApp::Render()
{
	spMainCamera->WipeOut();


	for (auto& model : vSpModels)
	{
		upModelDrawer->SetModel(model.get());
		upModelDrawer->SetCamera(spMainCamera.get());
		upModelDrawer->SetLightManager(spLightManager.get());
		Canvas<BaseModelDrawer> canvas(upModelDrawer.get());
		canvas.Render();
	}

	if (spSelectedModel)
	{
		upModelOutlineDrawer->SetModel(spSelectedModel.get());
		upModelOutlineDrawer->SetCamera(spMainCamera.get());
		Canvas<ModelOutlineDrawer> canvas(upModelOutlineDrawer.get());
		canvas.Render();
	}
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

			Update();
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

	ImGui_ImplDX11_Init(cpDevice.Get(), cpDeviceContext.Get());
    ImGui_ImplWin32_Init(hMainWindow);
}

void PortfolioApp::SetImGUIRendering()
{
	ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	upModelManageGui->RenderGui();
	upLightManageGui->RenderGui();
	upFileManageGui->RenderGui();
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

void PortfolioApp::ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	if (cpSwapChain)
	{
		uiWidth = uiWidthIn;
		uiHeight = uiHeightIn;
		fAspectRatio = uiWidth / (float)uiHeight;
		cpSwapChain->ResizeBuffers(0, uiWidth, uiHeight, DXGI_FORMAT_UNKNOWN, 0);
		spMainCamera->Resize(fAspectRatio);
	}
}

void PortfolioApp::CheckMouseHoveredModel()
{
	ModelIDData uiSelectedModelID = spMainCamera->GetPointedModelID();
	auto findResult = find_if(vSpModels.begin(), vSpModels.end(), [&](shared_ptr<ModelInterface> model) { return model->modelID.sIdData == uiSelectedModelID; });
	if (findResult != vSpModels.end())
	{
		spTempSelectedModel = *findResult;
	}
	else
	{
		spTempSelectedModel = nullptr;
	}
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
		ResizeSwapChain((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		return 0;
	case WM_MOUSEMOVE:
		spMainCamera->SetFromMouseXY(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
		if (spTempSelectedModel != nullptr)
		{
			spSelectedModel = spTempSelectedModel;
		}
		return 0;
	case WM_LBUTTONDOWN:
		// TODO : 모델 선택 관련 로직 추가
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case KeyCode::W:
			spMainCamera->StartMove(MoveDir::Forward);
			break;
		case KeyCode::A:
			spMainCamera->StartMove(MoveDir::Left);
			break;
		case KeyCode::S:
			spMainCamera->StartMove(MoveDir::Backward);
			break;
		case KeyCode::D:
			spMainCamera->StartMove(MoveDir::Right);
			break;
		case KeyCode::F:
			spMainCamera->SwitchFirstView();
			break;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam) {
		case KeyCode::W:
			spMainCamera->StopMove(MoveDir::Forward);
			break;
		case KeyCode::A:
			spMainCamera->StopMove(MoveDir::Left);
			break;
		case KeyCode::S:
			spMainCamera->StopMove(MoveDir::Backward);
			break;
		case KeyCode::D:
			spMainCamera->StopMove(MoveDir::Right);
			break;
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
