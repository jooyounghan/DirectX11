#include "ImguiManager.h"

ImguiManager::ImguiManager(const int& iWidthIn, const int& iHeightIn)
    : iWidth(iWidthIn), iHeight(iHeightIn)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.DisplaySize = ImVec2((float)iWidth, (float)iHeight);
    ImGui::StyleColorsLight();
}

ImguiManager::~ImguiManager()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImguiManager::InitImgui(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContex, HWND hWindow)
{
    ImGui_ImplDX11_Init(pDevice, pDeviceContex);
    ImGui_ImplWin32_Init(hWindow);
}

void ImguiManager::SetRender()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Scene Control");

    // 더나눌 수 있는 구조?


    ImGui::End();
    ImGui::Render();
}

void ImguiManager::Render()
{

}
