#include "PortfolioApp.h"
#include "ID3D11Helper.h"

#include "ModelManageGui.h"
#include "LightManageGui.h"
#include "SettingManageGui.h"
#include "CameraManageGui.h"
#include "FileManageGui.h"

#include "BaseModelDrawer.h"
#include "ModelOutlineDrawer.h"
#include "NormalVectorDrawer.h"
#include "CubeMapDrawer.h"
#include "MirrorDrawer.h"

#include "CameraInterface.h"
#include "CameraNormal.h"

#include "ModelInterface.h"
#include "ObjectModel.h"
#include "SphereModel.h"
#include "SquareModel.h"
#include "CubeMapModel.h"
#include "MirrorModel.h"

#include "ModelID.h"

#include "LightManager.h"

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
	
	// =================================================================================================
	// SDR
	//spMainCameras = make_shared<CameraNormal>(cpDevice, cpDeviceContext, cpSwapChain, uiWidth, uiHeight, uiNumLevelQuality);

	// HDR
	spMainCameras = make_shared<CameraNormal>(cpDevice, cpDeviceContext, cpSwapChain, uiWidth, uiHeight, uiNumLevelQuality, DXGI_FORMAT_R16G16B16A16_FLOAT);
	// =================================================================================================
	
	
	shared_ptr<SphereModel> spSphere = make_shared<SphereModel>(cpDevice, cpDeviceContext, 0.f, 0.f, 0.f, 2.f);
	vSpPickableModels.push_back(spSphere);
	vSpObjectModels.push_back(spSphere);

	shared_ptr<MirrorModel> spMirrorLeft = make_shared<MirrorModel>(cpDevice, cpDeviceContext, 5.f, 5.f, -5.f, 0.f, 0.f, spMainCameras);
	shared_ptr<MirrorModel> spMirrorRight = make_shared<MirrorModel>(cpDevice, cpDeviceContext, 5.f, 5.f, 5.f, 0.f, 0.f, spMainCameras);
	vSpPickableModels.push_back(spMirrorLeft);
	vSpPickableModels.push_back(spMirrorRight);
	vSpMirrorModels.push_back(spMirrorLeft);
	vSpMirrorModels.push_back(spMirrorRight);

	spCubeMap = make_shared<CubeMapModel>(cpDevice, cpDeviceContext);

	spLightManager = make_unique<LightManager>(cpDevice, cpDeviceContext);
	spLightManager->AddDirectionalLight(
		XMVectorSet(0.f, 0.f, -100.f, 1.f),
		XMVectorSet(1.f, 0.1f, 0.1f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f)
	);

	// GUI Ãß°¡ =====================================================================================
	vUpManageGuis.push_back(make_unique<ModelManageGui>(vSpPickableModels, spSelectedModel));
	vUpManageGuis.push_back(make_unique<LightManageGui>(spLightManager));
	vUpManageGuis.push_back(make_unique<SettingManageGui>(bIsNormalVectorDraw, bIsWireFrameDraw, spCubeMap));
	vUpManageGuis.push_back(make_unique<CameraManageGui>(spMainCameras));
	vUpManageGuis.push_back(make_unique<FileManageGui>(cpDevice, cpDeviceContext));
	// ==============================================================================================
	upModelDrawer = make_unique<BaseModelDrawer>(cpDevice, cpDeviceContext);
	upModelOutlineDrawer = make_unique<ModelOutlineDrawer>(cpDevice, cpDeviceContext);
	upNormalVectorDrawer = make_unique<NormalVectorDrawer>(cpDevice, cpDeviceContext);
	upCubeMapDrawer = make_unique<CubeMapDrawer>(cpDevice, cpDeviceContext);
	upMirrorDrawer = make_unique<MirrorDrawer>(cpDevice, cpDeviceContext);
	// ==============================================================================================
}

void PortfolioApp::Update(const float& fDelta)
{
	spMainCameras->Update(fDelta);
	spLightManager->Update();

	for (auto& model : vSpPickableModels)
	{
		model->Update(fDelta);
	}
	spCubeMap->Update(fDelta);
}

void PortfolioApp::Render()
{
	spMainCameras->WipeOut(DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f));

	for (auto& pMirror : vSpMirrorModels)
	{
		pMirror->WipeOut(DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f));
	}

	upModelDrawer->Draw(spMainCameras.get(), spLightManager.get(), vSpObjectModels, spCubeMap.get());

	if (bIsNormalVectorDraw)
	{
		for (auto& model : vSpPickableModels)
		{
			upNormalVectorDrawer->Draw(spMainCameras.get(), model.get());
		}
	}
	
	if (spSelectedModel)
	{
		upModelOutlineDrawer->Draw(spMainCameras.get(), spSelectedModel.get());
	}

	upCubeMapDrawer->Draw(spMainCameras.get(), spCubeMap.get());

	upMirrorDrawer->Draw(upModelDrawer.get(), upCubeMapDrawer.get(), spMainCameras.get(), spLightManager.get(), vSpObjectModels, spCubeMap.get(), vSpMirrorModels);

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
	ImGui_ImplDX11_Init(cpDevice.Get(), cpDeviceContext.Get());
    ImGui_ImplWin32_Init(hMainWindow);
}

void PortfolioApp::SetImGUIRendering()
{
	ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	for (auto& gui : vUpManageGuis)
	{
		gui->RenderGui();
	}
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
	auto findResult = find_if(vSpPickableModels.begin(), vSpPickableModels.end(), [&](shared_ptr<PickableModel> model) { return model->upModelID->sIdData == uiSelectedModelID; });
	if (findResult != vSpPickableModels.end())
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
		CheckMouseHoveredModel();
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
