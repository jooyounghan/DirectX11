#pragma once



#include <d3d11.h>
#include <wrl.h>

#include "ModelSelectDialog.h"
#include "LightSelectDialog.h"

using Microsoft::WRL::ComPtr;
using std::atomic;

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
	void RecordRendering(IN OUT struct atomic<float>& delta_time);
	void Render();

public:
	ModelSelectDialog m_model_select_dialog_;
	LightSelectDialog m_light_select_dialog_;

public:

};

