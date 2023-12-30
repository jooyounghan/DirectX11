#include "PortfolioApp.h"
#include "ID3D11Helper.h"

#include "ModelManageGui.h"
#include "LightManageGui.h"
#include "SettingManageGui.h"
#include "CameraManageGui.h"
#include "FileManageGui.h"


#include "PBRPointLightDrawer.h"
#include "PBRSpotLightDrawer.h"
#include "PointLightShadowDrawer.h"
#include "SpotLightShadowDrawer.h"

#include "NormalVectorDrawer.h"
#include "CubeMapDrawer.h"
#include "MirrorDrawer.h"

#include "CameraInterface.h"
#include "NormalCamera.h"
#include "BloomCamera.h"

#include "ModelInterface.h"
#include "PBRModel.h"
#include "SphereModel.h"
#include "SquareModel.h"
#include "PlaneModel.h"
#include "CubeMapModel.h"
#include "MirrorModel.h"

#include "ModelID.h"

#include "LightManager.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "DefineVar.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <iostream>

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

	InitImGUI();
	
	// =================================================================================================
	// SDR
	//spMainCameras = make_shared<NormalCamera>(cpDevice, cpDeviceContext, cpSwapChain, uiWidth, uiHeight, uiNumLevelQuality);

	// HDR
	spMainCamera = make_shared<BloomCamera>(
		cpDevice.Get(), cpDeviceContext.Get(), 
		uiWidth, uiHeight, uiNumLevelQuality, DXGI_FORMAT_R16G16B16A16_FLOAT,
		0.f, 5.f, -10.f, 70.f, 0.01f, 1000.f
	);
	spMainCamera->SetAsMainCamera(cpSwapChain.Get());
	// =================================================================================================
	
	
	shared_ptr<SphereModel> spSphere = make_shared<SphereModel>(cpDevice.Get(), cpDeviceContext.Get(), 0.f, 5.f, 0.f, 2.f);
	vSpPickableModels.push_back(spSphere);
	vSpPBRModels.push_back(spSphere);

	shared_ptr<PlaneModel> spPlane = make_shared<PlaneModel>(cpDevice.Get(), cpDeviceContext.Get(), 0.f, 0.f, 0.f, 100.f, 100.f, 10.f, 10.f);
	vSpPickableModels.push_back(spPlane);
	vSpPBRModels.push_back(spPlane);

	shared_ptr<MirrorModel> spMirrorLeft = make_shared<MirrorModel>(
		cpDevice.Get(), cpDeviceContext.Get(), 5.f, 5.f, 
		DXGI_FORMAT_R16G16B16A16_FLOAT, -5.f, 5.f, 0.f, spMainCamera
	);
	shared_ptr<MirrorModel> spMirrorRight = make_shared<MirrorModel>(
		cpDevice.Get(), cpDeviceContext.Get(), 5.f, 5.f, 
		DXGI_FORMAT_R16G16B16A16_FLOAT, 5.f, 5.f, 0.f, spMainCamera
	);
	vSpPickableModels.push_back(spMirrorLeft);
	vSpPickableModels.push_back(spMirrorRight);
	vSpMirrorModels.push_back(spMirrorLeft);
	vSpMirrorModels.push_back(spMirrorRight);

	spCubeMap = make_shared<CubeMapModel>(cpDevice.Get(), cpDeviceContext.Get());

	spLightManager = make_unique<LightManager>(cpDevice.Get(), cpDeviceContext.Get());

	// GUI Ãß°¡ =====================================================================================
	vUpManageGuis.push_back(make_unique<ModelManageGui>(vSpPickableModels, spSelectedModel));
	vUpManageGuis.push_back(make_unique<LightManageGui>(spLightManager.get()));
	vUpManageGuis.push_back(make_unique<SettingManageGui>(cpDevice.Get(), cpDeviceContext.Get(), bIsNormalVectorDraw, bIsWireFrameDraw, spCubeMap));
	vUpManageGuis.push_back(make_unique<CameraManageGui>(spMainCamera));
	vUpManageGuis.push_back(make_unique<FileManageGui>(cpDevice, cpDeviceContext));
	// ==============================================================================================
	upPointLightModelDrawer = make_unique<PBRPointLightDrawer>(cpDevice.Get(), cpDeviceContext.Get());
	upSpotLightModelDrawer = make_unique<PBRSpotLightDrawer>(cpDevice.Get(), cpDeviceContext.Get());
	upPointLightShadowDrawer = make_unique<PointLightShadowDrawer>(cpDevice.Get(), cpDeviceContext.Get());
	upSpotLightShadowDrawer = make_unique<SpotLightShadowDrawer>(cpDevice.Get(), cpDeviceContext.Get());
	upNormalVectorDrawer = make_unique<NormalVectorDrawer>(cpDevice.Get(), cpDeviceContext.Get());
	upCubeMapDrawer = make_unique<CubeMapDrawer>(cpDevice.Get(), cpDeviceContext.Get());
	upMirrorDrawer = make_unique<MirrorDrawer>(cpDevice.Get(), cpDeviceContext.Get());
	// ==============================================================================================
}

void PortfolioApp::Update(const float& fDelta)
{
	spMainCamera->Update(fDelta);
	spLightManager->Update();

	for (auto& model : vSpPickableModels)
	{
		model->Update(fDelta);
	}
	spCubeMap->Update(fDelta);
}

void PortfolioApp::Render()
{
	spMainCamera->WipeOut();

	for (auto& pMirror : vSpMirrorModels)
	{
		pMirror->WipeOut();
	}

	const vector<PointLight*> pPoints = spLightManager->GetPointLights();
	const vector<SpotLight*> pSpots = spLightManager->GetSpotLights();

	for (auto& pLight : pPoints)
	{
		upPointLightShadowDrawer->Draw(pLight, vSpPickableModels);
		upPointLightModelDrawer->Draw(spMainCamera.get(), pLight, vSpPickableModels);
		upMirrorDrawer->Draw(
			spMainCamera.get(), pLight,
			upPointLightModelDrawer.get(), vSpPBRModels,
			upCubeMapDrawer.get(), spCubeMap.get(),
			vSpMirrorModels
		);
	}

	for (auto& pLight : pSpots)
	{
		upSpotLightShadowDrawer->Draw(pLight, vSpPickableModels);
		upSpotLightModelDrawer->Draw(spMainCamera.get(), pLight, vSpPickableModels);
		upMirrorDrawer->Draw(
			spMainCamera.get(), pLight,
			upSpotLightModelDrawer.get(), vSpPBRModels,
			upCubeMapDrawer.get(), spCubeMap.get(),
			vSpMirrorModels
		);
	}

	if (bIsNormalVectorDraw)
	{
		for (auto& model : vSpPickableModels)
		{
			upNormalVectorDrawer->Draw(spMainCamera.get(), model.get());
		}
	}

	upCubeMapDrawer->Draw(spMainCamera.get(), spCubeMap.get());

	spMainCamera->DoPostProcess();
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
	if (cpSwapChain && spMainCamera)
	{
		uiWidth = uiWidthIn;
		uiHeight = uiHeightIn;
		fAspectRatio = uiWidth / (float)uiHeight;
		spMainCamera->ResetFromMainCamera();
		cpSwapChain->ResizeBuffers(0, uiWidth, uiHeight, DXGI_FORMAT_UNKNOWN, 0);
		spMainCamera->SetAsMainCamera(cpSwapChain.Get());
		spMainCamera->Resize(uiWidth, uiHeightIn, fAspectRatio);
	}
}

void PortfolioApp::CheckMouseHoveredModel()
{
	unsigned int uiSelectedModelIDAsRGBA = spMainCamera->GetPointedModelIDAsRGBA();
	auto findResult = find_if(
		vSpPickableModels.begin(), 
		vSpPickableModels.end(), 
		[&](shared_ptr<PickableModelInterface> model) 
		{ 
			return model->upModelID->IsRGBASameWithID(uiSelectedModelIDAsRGBA);
		}
	);
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
		spMainCamera->SetFromMouseXY(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
		return 0;
	case WM_LBUTTONDOWN:
		CheckMouseHoveredModel();
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			if (spSelectedModel != spTempSelectedModel)
			{
				spSelectedModel = spTempSelectedModel;
			}
			else
			{
				spSelectedModel = nullptr;
			}
		}
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
