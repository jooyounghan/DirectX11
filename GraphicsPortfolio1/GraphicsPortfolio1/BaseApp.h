#pragma once
#include <Windows.h>

class BaseApp
{
public:
	BaseApp();
	virtual ~BaseApp();

public:
	virtual bool Initialize() = 0;

protected:
	virtual bool InitWindowApp() = 0;

protected:
	int	m_screen_width_;
	int	m_screen_height_;
	HWND m_main_window_;

public:
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual float GetAspectRatio() = 0;

public:
	virtual int Run() = 0;
};

