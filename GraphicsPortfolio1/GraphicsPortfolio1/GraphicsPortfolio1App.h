#pragma once

#include "BaseApp.h"

#include "GameLoop.h"
#include "D3D11Utilizer.h"
#include "ImGuiManager.h"

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

protected:
	GameLoop		m_game_loop_;
	D3D11Utilizer	m_d3d11_utilizer;
	ImGuiManager	m_imgui_manager;

public:
	virtual int Run() override;

public:
	void SetImGuiDXDelegate();
};

