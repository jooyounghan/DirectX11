#include "PortfolioApp.h"

#include "ID3D11Helper.h"

#include "CameraInterface.h"

#include "Canvas.h"

#include "BaseModelDrawer.h"
#include "ModelOutlineDrawer.h"

#include "ModelInterface.h"

#include "LightManager.h"

#include "TempVariable.h"
#include "FileManager.h"

#include "DefineVar.h"

#include <string>

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
	ID3D11Helper::Init(cpDevice.Get(), cpDeviceContext.Get());
	ID3D11Helper::SetViewPort(0.f, 0.f, float(uiWidth), float(uiHeight), 0.f, 1.f, cpDeviceContext.Get(), &sScreenViewport);

	InitImGUI();

	upFileManager = make_unique<FileManager>(cpDevice, cpDeviceContext);
	upLightManager = make_unique<LightManager>(cpDevice, cpDeviceContext);
	upModelDrawer = make_unique<BaseModelDrawer>(cpDevice, cpDeviceContext);
	upModelOutlineDrawer = make_unique<ModelOutlineDrawer>(cpDevice, cpDeviceContext);

	// For Testing ==================================================================================
	upFileManager->LoadImageFromFile(L"..\\Texture\\GrassWithMudAndStone");

	spMainCamera = make_shared<CameraInterface>(cpDevice, cpDeviceContext, cpSwapChain, uiWidth, uiHeight, uiNumLevelQuality);
	spvModels.push_back(std::make_shared<ModelInterface>(cpDevice, cpDeviceContext, 0.f, 0.f, 0.f, 2.f));
	spvModels.push_back(std::make_shared<ModelInterface>(cpDevice, cpDeviceContext, 5.f, 0.f, 5.f, 2.f));

	upLightManager->AddDirectionalLight(
		XMVectorSet(0.f, 0.f, -100.f, 1.f),
		XMVectorSet(1.f, 0.1f, 0.1f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f)
	);
	// ==============================================================================================
}

void PortfolioApp::Update()
{
	spMainCamera->Update();
	upLightManager->Update();

	CheckMouseHoveredModel();

	for (auto& model : spvModels)
	{
		model->Update();
	}
}

void PortfolioApp::Render()
{
	spMainCamera->WipeOut();


	for (auto& model : spvModels)
	{
		upModelDrawer->SetModel(model.get());
		upModelDrawer->SetCamera(spMainCamera.get());
		upModelDrawer->SetLightManager(upLightManager.get());
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

    UpdateGUI(); // 추가적으로 사용할 GUI
    ImGui::Render();
}

void PortfolioApp::UpdateGUI()
{
	SetModelManageWnd();
	SetLightManageWnd();
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

void PortfolioApp::SetModelManageWnd()
{
	ImGui::Begin("Model Manager");
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	bool bModelNotSelected = (spSelectedModel == nullptr);
	ImGui::BeginDisabled(bModelNotSelected);
	ImGui::SliderFloat3("Scale Vector", bModelNotSelected ? TempVariable::fTempFloat3 : spSelectedModel->sTransformationProperties.xmvScale.m128_f32, 0.f, 5.f);
	ImGui::SliderFloat3("Rotation Vector", bModelNotSelected ? TempVariable::fTempFloat3 : (float*)(&spSelectedModel->sTransformationProperties.sPositionAngle), -2.f * XM_PI, 2.f * XM_PI);
	ImGui::SliderFloat3("Translation Vector", bModelNotSelected ? TempVariable::fTempFloat3 : spSelectedModel->sTransformationProperties.xmvTranslation.m128_f32, -10.f, 10.f);
	ImGui::EndDisabled();
	ImGui::End();
}

void PortfolioApp::SetLightManageWnd()
{
	ImGui::Begin("Light Mananger");

	if (ImGui::CollapsingHeader("Add Light"))
	{
		SetLightAddMenu();
	}

	if (ImGui::CollapsingHeader("Select Light"))
	{
		SetLightSelectorMenu();
	}

	if (ImGui::CollapsingHeader("Setting"))
	{
		SetLightSettingMenu();
	}
	ImGui::End();
}

void PortfolioApp::SetLightAddMenu()
{
	static bool	lightTypeCheckFlag[3] = { false, false, false };
	static const char* items[] = { "Directional", "Point", "Spot" };
	static int selected_idx = 0;
	if (ImGui::BeginCombo("Select Light Type", items[selected_idx], NULL))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			const bool is_selected = (selected_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
				selected_idx = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (selected_idx == LightType::Directional)	SetDirectionalLightMenu(&LightManager::sTempLightSet);
	else if (selected_idx == LightType::Point)	SetPointLightMenu(&LightManager::sTempLightSet);
	else if (selected_idx == LightType::Spot)	SetSpotLightMenu(&LightManager::sTempLightSet);
	else;

	const float& fWindowSize = ImGui::GetWindowWidth();
	if (ImGui::Button("Add", ImVec2(fWindowSize / 2.f, 0.f)))
	{
		switch (selected_idx)
		{
		case LightType::Directional:
			upLightManager->AddDirectionalLight(
				LightManager::sTempLightSet.xmvLocation,
				LightManager::sTempLightSet.xmvLightColor,
				LightManager::sTempLightSet.xmvDirection
			);
			break;
		case LightType::Point:
			break;
		case LightType::Spot:
			break;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset", ImVec2(fWindowSize / 2.f, 0.f)))
	{
		AutoZeroMemory(LightManager::sTempLightSet);
	}
}

void PortfolioApp::SetLightSelectorMenu()
{
	if (ImGui::BeginTable("Light Selector", 3, ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Check");
		ImGui::TableSetupColumn("Light ID");
		ImGui::TableSetupColumn("Light Type");
		ImGui::TableHeadersRow();

		const vector<LightSet>& vLights = upLightManager->GetLights();
		const unsigned short& usSelectedLightIndex = upLightManager->GetSelectedLightIndex();
		for (size_t idx = 0; idx < vLights.size(); ++idx)
		{
			bool IsSelected = ((usSelectedLightIndex - 1) == idx);
			ImGui::TableNextRow();
			ImGui::PushID(&vLights[idx]);
			ImGui::TableSetColumnIndex(0);
			ImGui::Checkbox(("Light" + to_string(idx + 1)).c_str(), &IsSelected);

			if (IsSelected)
			{
				upLightManager->SetSelectedLightIndex(idx + 1);
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", idx + 1);
			ImGui::TableSetColumnIndex(2);
			string strLightType;
			switch (vLights[idx].eLightType)
			{
			case LightType::Directional:
				strLightType = "Directional";
				break;
			case LightType::Point:
				strLightType = "Point";
				break;
			case LightType::Spot:
				strLightType = "Spot";
				break;
			case LightType::NotALight:
			default:
				break;
			}
			ImGui::Text(strLightType.c_str());
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}

void PortfolioApp::SetLightSettingMenu()
{
	const vector<LightSet>& vLights = upLightManager->GetLights();
	unsigned short usSelectedLighIndex = upLightManager->GetSelectedLightIndex();
	bool bLightNotSelected = ((vLights.size() == 0) || (usSelectedLighIndex == 0));
	if (!bLightNotSelected)
	{
		usSelectedLighIndex = usSelectedLighIndex - 1;
		LightType eLightType = vLights[usSelectedLighIndex].eLightType;
		LightSet* pLightSet = const_cast<LightSet*>(&vLights[usSelectedLighIndex]);
		if (eLightType == LightType::Directional)	SetDirectionalLightMenu(pLightSet);
		else if (eLightType == LightType::Point)	SetPointLightMenu(pLightSet);
		else if (eLightType == LightType::Spot)		SetSpotLightMenu(pLightSet);
		else;
	}
	else
	{
		ImGui::BulletText("Select The Light For the Setting");
	}
}

void PortfolioApp::SetDirectionalLightMenu(LightSet* pLightSet)
{
	ImGui::PushID(pLightSet);
	ImGui::SliderFloat3("Light Location", pLightSet->xmvLocation.m128_f32, -10.f, 10.f);
	ImGui::SliderFloat3("Light Color", pLightSet->xmvLightColor.m128_f32, 0, 1.f);
	ImGui::SliderFloat3("Light Direction", pLightSet->xmvDirection.m128_f32, -1.f, 1.f);
	ImGui::SliderFloat("Light Power", &pLightSet->fLightStrength, 0.f, 10.f);
	ImGui::PopID();
}

void PortfolioApp::SetPointLightMenu(LightSet* pLightSet)
{
	ImGui::PushID(pLightSet);
	ImGui::SliderFloat3("Light Location", pLightSet->xmvLocation.m128_f32, -100.f, 100.f);
	ImGui::SliderFloat3("Light Color", pLightSet->xmvLightColor.m128_f32, 0, 1.f);
	ImGui::SliderFloat("Light Power", &pLightSet->fLightStrength, 0.f, 10.f);
	ImGui::SliderFloat2("Fall Off Start/End", &pLightSet->fFallOffStart, 0.f, 100.f);
	ImGui::PopID();
}

void PortfolioApp::SetSpotLightMenu(LightSet* pLightSet)
{
	ImGui::PushID(pLightSet);
	ImGui::SliderFloat3("Light Location", pLightSet->xmvLocation.m128_f32, -10.f, 10.f);
	ImGui::SliderFloat3("Light Color", pLightSet->xmvLightColor.m128_f32, 0, 1.f);
	ImGui::SliderFloat3("Light Direction", pLightSet->xmvDirection.m128_f32, -1.f, 1.f);
	ImGui::SliderFloat("Light Power", &pLightSet->fLightStrength, 0.f, 10.f);
	ImGui::SliderFloat("Spot Power", &pLightSet->fSpotPower, 0.f, 10.f);
	ImGui::PopID();
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
	auto findResult = find_if(spvModels.begin(), spvModels.end(), [&](shared_ptr<ModelInterface> model) { return model->modelID.sIdData == uiSelectedModelID; });
	if (findResult != spvModels.end())
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
