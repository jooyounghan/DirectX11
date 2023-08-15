
#include <iostream>
#include <memory>
#include <mutex>

#include "PortfolioApp.h"


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::mutex mtxConsole;
void Console(const char* text)
{
    std::lock_guard<std::mutex> lockGuard(mtxConsole);
    std::cout << text << std::endl;
}

int main() {

    int iWidth = 1280;
    int iHeight = 960;

    WNDCLASSEX wc = { sizeof(WNDCLASSEX),
                     CS_CLASSDC,
                     WndProc,
                     0L,
                     0L,
                     GetModuleHandle(NULL),
                     NULL,
                     NULL,
                     NULL,
                     NULL,
                     L"Portfolio1", // lpszClassName, L-string
                     NULL };

    RegisterClassEx(&wc);

    RECT rWindowRect = { 0, 0, iWidth, iHeight };

    AdjustWindowRect(&rWindowRect, WS_OVERLAPPEDWINDOW, FALSE);


    HWND hWindow = CreateWindow(wc.lpszClassName, L"Joo YH / Portfolio1",
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        rWindowRect.right - rWindowRect.left,
        rWindowRect.bottom - rWindowRect.top,
        NULL, NULL, wc.hInstance, NULL);

    ShowWindow(hWindow, SW_SHOWDEFAULT);
    UpdateWindow(hWindow);

    // 포트폴리오 어플리케이션 생성
    PortfolioApp pApp = PortfolioApp(hWindow, iWidth, iHeight);
    pApp.Init();

    // Main message loop
    MSG msg = {};
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            pApp.Update();
            pApp.Render();
            pApp.SwapChain();
        }
    }

    DestroyWindow(hWindow);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

// Windows procedure
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        // Reset and resize swapchain
        return 0;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
