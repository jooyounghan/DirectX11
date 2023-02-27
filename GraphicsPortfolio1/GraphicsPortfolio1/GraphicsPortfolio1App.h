#pragma once

#include "BaseApp.h"
#include "CommandManager.h"
#include "ImGuiManager.h"
#include "Stage.h"


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
	Stage			m_stage_;
	ImGuiManager	m_imgui_manager;

public:
	atomic<float>	m_delta_time_;

protected:
	CommandManager command_manager_;

public:
	virtual int Run() override;

public:
	void SetDelegation();
};

