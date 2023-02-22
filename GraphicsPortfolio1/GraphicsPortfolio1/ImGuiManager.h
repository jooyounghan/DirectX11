#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <wrl.h>

#include <string>
#include <atomic>
#include <memory>

#include "Delegator.h"

using std::string;
using Microsoft::WRL::ComPtr;
using std::atomic;
using std::shared_ptr;

struct ModelData
{
	bool is_checked = false;
	string base_path;
	string file_name;
	float model_translation[3]{ 0.f, 0.f, 0.f };
	float model_rotation[3]{ 0.f, 0.f, 0.f };
	float model_scaling[3]{ 0.f, 0.f, 0.f };
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

public:
	bool InitImGui(HWND main_window, ComPtr<ID3D11Device> m_device, ComPtr<ID3D11DeviceContext> m_context);
public:
	void ReSetImGui();

public:
	void RecordRendering(IN OUT atomic<float>& delta_time);
	void Render();

public:
	void CreateModelSelectFrame(const float& delta_time);

public:
	static void HelpMarker(const char* marker_text, const char* desc);

public:
	std::vector<shared_ptr<ModelData>>	m_model_files_;
	shared_ptr<ModelData>				m_selected_model_;

public:
	float m_translation_[3];
	float m_rotation_[3];
	float m_scaling_[3];

public:
	Delegator<void, const string&, const string&>	m_on_file_added_;
	Delegator<void, const size_t&>					m_on_file_deleted_;

public:
	bool											m_model_transfrom_flag_;
	//Delegator<void, const size_t&>					m_on_file_deleted_;
};

