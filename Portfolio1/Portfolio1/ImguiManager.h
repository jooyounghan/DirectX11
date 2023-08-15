#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <windows.h>

class ImguiManager
{
public:
	ImguiManager(const int& iWidthIn, const int& iHeightIn);
    ~ImguiManager();

public:
    void InitImgui(ID3D11Device* idDevice, ID3D11DeviceContext* idcDeviceContex, HWND hWindow);
    void SetRender();
    void Render();

private:
    int iWidth;
    int iHeight;
};

