#include "PortfolioApp.h"
#include "ID3D11Helper.h"

#include "ModelManageGui.h"
#include "LightManageGui.h"
#include "FileManageGui.h"
#include "SettingManageGui.h"

#include "BaseModelDrawer.h"
#include "ModelOutlineDrawer.h"
#include "NormalVectorDrawer.h"

#include "Canvas.h"

#include "CameraInterface.h"
#include "CameraUNorm.h"

#include "ModelInterface.h"
#include "SphereModel.h"
#include "SquareModel.h"

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
	: BaseApp(uiWidthIn, uiHeightIn), spMainCameras(nullptr), spSelectedModel(nullptr), spTempSelectedModel(nullptr)
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

	InitImGUI();

	// GUI 추가 =====================================================================================
	upModelManageGui = make_unique<ModelManageGui>(vSpModels, spSelectedModel, spTempSelectedModel);
	upLightManageGui = make_unique<LightManageGui>(spLightManager);
	upFileManageGui = make_unique<FileManageGui>(upFileManager);
	upSettingManageGui = make_unique<SettingManageGui>();
	// ==============================================================================================

	upFileManager = make_unique<FileManager>(cpDevice, cpDeviceContext);
	spLightManager = make_unique<LightManager>(cpDevice, cpDeviceContext);

	upBaseCanvas = make_unique<Canvas<BaseModelDrawer>>();
	upModelOutlineCanvas = make_unique<Canvas<ModelOutlineDrawer>>();
	upNVCanvas = make_unique<Canvas<NormalVectorDrawer>>();

	upModelDrawer = make_unique<BaseModelDrawer>(cpDevice, cpDeviceContext);
	upModelOutlineDrawer = make_unique<ModelOutlineDrawer>(cpDevice, cpDeviceContext);
	upNormalVectorDrawer = make_unique<NormalVectorDrawer>(cpDevice, cpDeviceContext);

	// For Testing ==================================================================================
	upFileManager->LoadImageFromFile(L".\\Texture\\GrassWithMudAndStone");

	spMainCameras = make_shared<CameraUNorm>(cpDevice, cpDeviceContext, cpSwapChain, uiWidth, uiHeight, uiNumLevelQuality);

	vSpModels.push_back(std::make_shared<SquareModel>(cpDevice, cpDeviceContext, 0.f, 0.f, 0.f, 2.f));
	vSpModels.push_back(std::make_shared<SphereModel>(cpDevice, cpDeviceContext, 5.f, 0.f, 5.f, 2.f));

	spLightManager->AddDirectionalLight(
		XMVectorSet(0.f, 0.f, -100.f, 1.f),
		XMVectorSet(1.f, 0.1f, 0.1f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f)
	);
	// ==============================================================================================
}

void PortfolioApp::Update()
{
	spMainCameras->Update();
	spLightManager->Update();

	CheckMouseHoveredModel();

	for (auto& model : vSpModels)
	{
		model->Update();
	}
}

void PortfolioApp::Render()
{
	spMainCameras->WipeOut(DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f));

	upBaseCanvas->SetDrawer(upModelDrawer.get());
	upModelOutlineCanvas->SetDrawer(upModelOutlineDrawer.get());
	upNVCanvas->SetDrawer(upNormalVectorDrawer.get());

	upModelDrawer->SetCameraAdapter(spMainCameras.get());
	upNormalVectorDrawer->SetCameraAdapter(spMainCameras.get());
	upModelOutlineDrawer->SetCameraAdapter(spMainCameras.get());

	for (auto& model : vSpModels)
	{
		upModelDrawer->SetModel(model.get());
		upModelDrawer->SetLightManager(spLightManager.get());
		upBaseCanvas->Render();
	}

	if (upSettingManageGui->IsNormalVectorDraw())
	{
		for (auto& model : vSpModels)
		{
			upNormalVectorDrawer->SetModel(model.get());
			upNVCanvas->Render();
		}
	}

	if (spSelectedModel)
	{
		upModelOutlineDrawer->SetModel(spSelectedModel.get());
		upModelOutlineCanvas->Render();
	}
	spMainCameras->DoPostProcess();
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
	upSettingManageGui->RenderGui();
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
		spMainCameras->Resize(fAspectRatio);
	}
}

void PortfolioApp::CheckMouseHoveredModel()
{
	ModelIDData uiSelectedModelID = spMainCameras->GetPointedModelID();
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
		spMainCameras->SetFromMouseXY(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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
			spMainCameras->StartMove(MoveDir::Forward);
			break;
		case KeyCode::A:
			spMainCameras->StartMove(MoveDir::Left);
			break;
		case KeyCode::S:
			spMainCameras->StartMove(MoveDir::Backward);
			break;
		case KeyCode::D:
			spMainCameras->StartMove(MoveDir::Right);
			break;
		case KeyCode::F:
			spMainCameras->SwitchFirstView();
			break;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam) {
		case KeyCode::W:
			spMainCameras->StopMove(MoveDir::Forward);
			break;
		case KeyCode::A:
			spMainCameras->StopMove(MoveDir::Left);
			break;
		case KeyCode::S:
			spMainCameras->StopMove(MoveDir::Backward);
			break;
		case KeyCode::D:
			spMainCameras->StopMove(MoveDir::Right);
			break;
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
