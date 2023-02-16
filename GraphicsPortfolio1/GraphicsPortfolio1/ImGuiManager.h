#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <wrl.h>

#include <string>

#include "Delegator.h"

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
	void RecordRendering(const float& delta_time);
	void Render();

public:
	void SetImGui(const float& delta_time);

public:
	std::vector<std::pair<bool, std::string>> items
	{
		{false, "Hello1"},
		{false, "Hello2"},
		{false, "Hello3"},
		{false, "Hello4"},
		{false, "Hello5"},
		{false, "Hello6"},
	};

public:
	Delegator<void, const std::string&> OnModelFileChanged;
};

