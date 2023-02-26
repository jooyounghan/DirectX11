
#include <directxtk/SimpleMath.h>

#include <Windows.h>
#include <atomic>

#include "ImGuiManager.h"

using namespace std;
using namespace DirectX::SimpleMath;

ImGuiManager::ImGuiManager()

{
}

ImGuiManager::~ImGuiManager()
{

}


bool ImGuiManager::InitImGui(HWND main_window, ComPtr<ID3D11Device> m_device, ComPtr<ID3D11DeviceContext> m_context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get())) {
        return false;
    }

    if (!ImGui_ImplWin32_Init(main_window)) {
        return false;
    }


    return true;
}

void ImGuiManager::ReSetImGui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::RecordRendering(IN OUT atomic<float>& delta_time)
{
    delta_time.store(1.f / ImGui::GetIO().Framerate);

    ImGui_ImplDX11_NewFrame(); // GUI 프레임 시작
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_model_select_dialog_.CreateModelSelector(delta_time.load());
    m_light_select_dialog_.CreateLightSelector(delta_time.load());

    ImGui::Render();
}

void ImGuiManager::Render()
{
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
