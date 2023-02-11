#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class ImGuiManager
{
public:
	ImGuiManager(int& imgui_width, int& imgui_height);
	~ImGuiManager();

public:
	int& m_screen_width_;
	int& m_screen_height_;

public:
	int m_imgui_width_;

public:
	int GetImGuiWidth();

public:
	bool InitImGui(HWND main_window, ComPtr<ID3D11Device> m_device, ComPtr<ID3D11DeviceContext> m_context);
public:
	void ReSetImGui();

public:
	void RecordRendering();
	void Render();

public:
	void SetImGui();
};

