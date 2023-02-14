
#include <Windows.h>

#include "ImGuiManager.h"



ImGuiManager::ImGuiManager(int& screen_width, int& screen_height)
    : m_screen_width_(screen_width), m_screen_height_(screen_height), m_imgui_width_(0)
{
}

ImGuiManager::~ImGuiManager()
{

}

int ImGuiManager::GetImGuiWidth()
{
    return m_imgui_width_;
}

bool ImGuiManager::InitImGui(HWND main_window, ComPtr<ID3D11Device> m_device, ComPtr<ID3D11DeviceContext> m_context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.DisplaySize = ImVec2(float(m_screen_width_), float(m_screen_height_));
    ImGui::StyleColorsLight();

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

void ImGuiManager::RecordRendering(const float& delta_time)
{
    ImGui_ImplDX11_NewFrame(); // GUI 프레임 시작
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame(); // 어떤 것들을 렌더링 할지 기록 시작
    ImGui::Begin("Scene Control");

    SetImGui(delta_time);

    ImGui::End();
    ImGui::Render(); // 렌더링할 것들 기록 끝
}

void ImGuiManager::Render()
{
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링
}

void ImGuiManager::SetImGui(const float& delta_time)
{
    ImGui::Text("Average %.3f ms/frame (%.1f FPS) With Game Loop",
        delta_time * 1000.0f, 1.f / delta_time);
    ImGui::Text("Average %.3f ms/frame (%.1f FPS) With ImGui GetIO",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
    m_imgui_width_ = int(ImGui::GetWindowWidth());
    ImGui::SetWindowSize(
        ImVec2((float)m_imgui_width_, (float)m_screen_height_));



}
