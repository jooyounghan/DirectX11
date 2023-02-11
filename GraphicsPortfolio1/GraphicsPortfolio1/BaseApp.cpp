#include "BaseApp.h"
#include <iostream>

#pragma region Global Variable And Function
BaseApp* g_baseApp;
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_baseApp->MsgProc(hWnd, msg, wParam, lParam);
}
#pragma endregion


BaseApp::BaseApp()
    : m_screen_width_(1280), m_screen_height_(960), m_main_window_(0)
{
	g_baseApp = this;
}

BaseApp::~BaseApp()
{
    g_baseApp = nullptr;
    DestroyWindow(m_main_window_);
}