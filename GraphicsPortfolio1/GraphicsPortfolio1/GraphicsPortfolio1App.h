#pragma once
#include <chrono>

#include "BaseApp.h"
#include "D3D11Utilizer.h"
#include "ImGuiManager.h"

using std::chrono::system_clock;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class GraphicsPortfolio1App : public BaseApp
{
public:
	GraphicsPortfolio1App();
	virtual ~GraphicsPortfolio1App() override;

public:
	virtual bool Initialize() override;

protected:
	virtual bool InitWindowApp() override;

public:
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	virtual float GetAspectRatio() override;

public:
	system_clock::time_point m_message_time_point_;
	system_clock::time_point m_update_time_point_;

public:
	float UpdateAndCalcDt(system_clock::time_point& time_point);

protected:
	D3D11Utilizer	m_d3d11_utilizer;
	ImGuiManager	m_imgui_manager;

public:
	virtual int Run() override;
};

