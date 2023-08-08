
#include <iostream>
#include <memory>

#include "ImguiManager.h"
#include "PortfolioApp.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

constexpr int iWidth = 1280;
constexpr int iHeight = 960;

int main() {


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


    HWND hwnd = CreateWindow(wc.lpszClassName, L"Joo YH / Portfolio1",
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        rWindowRect.right - rWindowRect.left,
        rWindowRect.bottom - rWindowRect.top,
        NULL, NULL, wc.hInstance, NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // 포트폴리오 어플리케이션 생성
    PortfolioApp pApp;

    // Imgui 생성
    // 포트폴리오 어플 안에 Imgui와 ID3D11 관련된 클래스를 넣으면?
    ImguiManager imgui(iWidth, iHeight);
    imgui.InitImgui(pApp.GetDevice(), pApp.GetDeviceContext(), hwnd);

    // Main message loop
    MSG msg = {};
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            imgui.SetRender();
            pApp.Update();
            pApp.Render();
            imgui.Render();
            pApp.SwapChain();
        }
    }

    DestroyWindow(hwnd);
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
