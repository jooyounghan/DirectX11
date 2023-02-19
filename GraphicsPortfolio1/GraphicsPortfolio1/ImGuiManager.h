#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <wrl.h>

#include <string>

#include "Delegator.h"

using std::string;
using Microsoft::WRL::ComPtr;

struct FileListItem
{
	bool is_checked;
	string base_path;
	string file_name;
};


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImGuiManager
{
public:
	ImGuiManager(UINT& imgui_width, UINT& imgui_height);
	~ImGuiManager();

public:
	UINT& m_screen_width_;
	UINT& m_screen_height_;

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
	std::vector<FileListItem> m_model_files_;

public:
	Delegator<void, const string&, const string&>	m_on_file_added_;
	Delegator<void, const size_t&>					m_on_file_deleted_;
};

