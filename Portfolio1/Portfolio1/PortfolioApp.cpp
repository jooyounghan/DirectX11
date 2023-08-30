#include "PortfolioApp.h"

#include "ICamera.h"

#include "IModel.h"
#include "TestModel.h"

#include "ILight.h"
#include "DirectionalLight.h"

#include "TempVariable.h"
#include "FileLoader.h"

#include <string>

using namespace DirectX;

using namespace std;

PortfolioApp::PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn)
	: BaseApp(uiWidthIn, uiHeightIn), pMainCamera(nullptr), pSelectedModel(nullptr)
{
	BaseApp::GlobalBaseApp = this;
}

PortfolioApp::~PortfolioApp()
{

}

void PortfolioApp::Init()
{
	FileLoader::PreLoadFiles();

	BaseApp::Init();
	InitImGUI();

	if (ICamera::DefaultCamera == nullptr)
	{
		ICamera::DefaultCamera = std::make_shared<ICamera>(cpDevice, cpDeviceContext, cpSwapChain, uiWidth, uiHeight, uiNumLevelQuality);
	}

	ILight::InitLights(cpDevice.Get(), cpDeviceContext.Get());

	// For Testing ==================================================================================

	pMainCamera = ICamera::DefaultCamera;
	vModels.push_back(std::make_shared<TestModel>(cpDevice, cpDeviceContext, 0.f, 0.f, 0.f, 2.f));
	vModels.push_back(std::make_shared<TestModel>(cpDevice, cpDeviceContext, 5.f, 0.f, 5.f, 2.f));
	pSelectedModel = vModels[1];

	umLights.emplace(
		std::make_shared<DirectionalLight>(
		cpDevice, cpDeviceContext,
		XMVectorSet(0.f, 100.f, 0.f, 1.f),
		XMVectorSet(1.f, 0.1f, 0.1f, 1.f),
		XMVectorSet(0.f, -1.f, 0.f, 0.f)
		),
		false
	);
	umLights.emplace(
		std::make_shared<DirectionalLight>(
			cpDevice, cpDeviceContext,
			XMVectorSet(100.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.1f, 1.0f, 0.1f, 1.f),
			XMVectorSet(-1.f, 0.f, 0.f, 0.f)
		),
		false
	);
	// ==============================================================================================
}

void PortfolioApp::Update()
{
	pMainCamera->Update();

	ILight::UpdateLights(cpDeviceContext.Get());

	for (auto& model : vModels)
	{
		model->Update();
	}
}

void PortfolioApp::Render()
{
	pMainCamera->WipeOut();

	for (auto& model : vModels)
	{
		model->Render();
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
    ImGui::StyleColorsLight();

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

	bool bModelNotSelected = (pSelectedModel == nullptr);
	ImGui::BeginDisabled(bModelNotSelected);
	ImGui::SliderFloat3("Scale Vector", bModelNotSelected ? TempVariable::fTempFloat3 : pSelectedModel->sModelTransformation.xmvScale.m128_f32, 0.f, 5.f);
	ImGui::SliderFloat3("Rotation Vector", bModelNotSelected ? TempVariable::fTempFloat3 : (float*)(&pSelectedModel->sModelTransformation.sPositionAngle), -2.f * XM_PI, 2.f * XM_PI);
	ImGui::SliderFloat3("Translation Vector", bModelNotSelected ? TempVariable::fTempFloat3 : pSelectedModel->sModelTransformation.xmvTranslation.m128_f32, -10.f, 10.f);
	ImGui::EndDisabled();
	ImGui::End();
}

void PortfolioApp::SetLightManageWnd()
{
	ImGui::Begin("Light Mananger");

	if (ImGui::BeginTable("Light Selector", 3, ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Check");
		ImGui::TableSetupColumn("Light ID");
		ImGui::TableSetupColumn("Light Type");
		ImGui::TableHeadersRow();

		for (auto& elemLightBool : umLights)
		{
			auto& pLight = elemLightBool.first;
			auto& bSelected = elemLightBool.second;

			ImGui::TableNextRow();
			ImGui::PushID(pLight.get());
			ImGui::TableSetColumnIndex(0);
			ImGui::Checkbox(("Light" + to_string(pLight->ullLightId)).c_str(), &bSelected);

#pragma region 체크박스 선택 처리
			if (bSelected == true)
			{
				if (pSelectedLight != nullptr && pSelectedLight != pLight)
				{
					if (umLights.find(pSelectedLight) != umLights.end())
					{
						umLights[pSelectedLight] = false;
					}

				}
				pSelectedLight = pLight;
			}
			else
			{
				if (pSelectedLight != nullptr && pSelectedLight == pLight)
				{
					pSelectedLight = nullptr;
				}
			}
#pragma endregion

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", pLight->ullLightId);
			ImGui::TableSetColumnIndex(2);
			string strLightType;
			switch (pLight->GetLightType())
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


	bool bLightNotSelected = (pSelectedLight == nullptr);
	ImGui::BeginDisabled(bLightNotSelected);
	ImGui::Text((pSelectedLight == nullptr) ? "" : to_string(pSelectedLight->ullLightId).c_str());
	ImGui::EndDisabled();

	ImGui::End();
}

void PortfolioApp::ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	if (cpSwapChain)
	{
		uiWidth = uiWidthIn;
		uiHeight = uiHeightIn;
		fAspectRatio = uiWidth / (float)uiHeight;
		cpSwapChain->ResizeBuffers(0, uiWidth, uiHeight, DXGI_FORMAT_UNKNOWN, 0);
		pMainCamera->Resize(fAspectRatio);
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
		pMainCamera->SetFromMouseXY(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONDOWN:
		// TODO : 모델 선택 관련 로직 추가
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case KeyCode::W:
			pMainCamera->StartMove(MoveDir::Forward);
			break;
		case KeyCode::A:
			pMainCamera->StartMove(MoveDir::Left);
			break;
		case KeyCode::S:
			pMainCamera->StartMove(MoveDir::Backward);
			break;
		case KeyCode::D:
			pMainCamera->StartMove(MoveDir::Right);
			break;
		case KeyCode::F:
			pMainCamera->SwitchFirstView();
			break;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam) {
		case KeyCode::W:
			pMainCamera->StopMove(MoveDir::Forward);
			break;
		case KeyCode::A:
			pMainCamera->StopMove(MoveDir::Left);
			break;
		case KeyCode::S:
			pMainCamera->StopMove(MoveDir::Backward);
			break;
		case KeyCode::D:
			pMainCamera->StopMove(MoveDir::Right);
			break;
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
