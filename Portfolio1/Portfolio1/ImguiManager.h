#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <windows.h>

class ImguiManager
{
public:
	ImguiManager(const int& iWidthIn, const int& iHeightIn)
        : iWidth(iWidthIn), iHeight(iHeightIn)
	{
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.DisplaySize = ImVec2((float)iWidth, (float)iHeight);
        ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends

	}
    ~ImguiManager()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

public:
    void InitImgui(ID3D11Device* idDevice, ID3D11DeviceContext* idcDeviceContex, HWND hWindow)
    {
        ImGui_ImplDX11_Init(idDevice, idcDeviceContex);
        ImGui_ImplWin32_Init(hWindow);
    }

    void SetRender()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Scene Control");

        // 더나눌 수 있는 구조?


        ImGui::End();
        ImGui::Render();
    }

    void Render()
    {

    }
private:
    int iWidth;
    int iHeight;
};

