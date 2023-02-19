#include "GraphicsPortfolio1App.h"
#include "D3D11Utilizer.h"

GraphicsPortfolio1App::GraphicsPortfolio1App()
    : BaseApp(),
    m_stage_(m_screen_width_, m_screen_height_),
    m_imgui_manager(m_screen_width_, m_screen_height_)
{
}

GraphicsPortfolio1App::~GraphicsPortfolio1App()
{
}

bool GraphicsPortfolio1App::Initialize()
{
    if (!InitWindowApp())
    {
        return false;
    }

    if (!m_stage_.InitStage(m_main_window_))
    {
        return false;
    }

    if (!m_imgui_manager.InitImGui(m_main_window_, m_stage_.GetDevice().Get(), m_stage_.GetDeviceContext().Get()))
    {
        return false;
    }

    SetImGuiDXDelegate();

    command_manager_.SetDefaultCommand(m_stage_.m_main_camera_);

    return true;
}

bool GraphicsPortfolio1App::InitWindowApp()
{
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
             L"GraphicsPortfolio1", // lpszClassName, L-string
             NULL };


    if (!RegisterClassEx(&wc))
    {
        cout << "RegisterClassEx() failed. Error Code : " << GetLastError() << endl;
        return false;
    }

    RECT wr = { 0, 0, m_screen_width_, m_screen_height_ };

    // 필요한 윈도우 크기(해상도) 계산
    // wr의 값이 바뀜
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

    // 윈도우를 만들때 위에서 계산한 wr 사용
    m_main_window_ = CreateWindow(wc.lpszClassName, L"GraphicsPortfolio1",
        WS_OVERLAPPEDWINDOW,
        100, // 윈도우 좌측 상단의 x 좌표
        100, // 윈도우 좌측 상단의 y 좌표
        wr.right - wr.left, // 윈도우 가로 방향 해상도
        wr.bottom - wr.top, // 윈도우 세로 방향 해상도
        NULL, NULL, wc.hInstance, NULL);

    if (!m_main_window_)
    {
        cout << "CreateWindow() failed. Error Code : " << GetLastError() << endl;
        return false;
    }

    // Set Timer for Managing Update Delta Time
    SetTimer(m_main_window_, 0, 16, NULL);

    ShowWindow(m_main_window_, SW_SHOWDEFAULT);
    UpdateWindow(m_main_window_);

    return true;
}

LRESULT GraphicsPortfolio1App::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;
    
    const float delta_time = m_game_loop_.GetElapsed("Message");
    switch (msg)
    {
    case WM_CREATE:
        m_game_loop_.AddTimePoint("Message");
        return 0;
    case WM_SIZE:
        if (m_stage_.IsSwappable())
        {
            m_screen_width_ = UINT(LOWORD(lParam));
            m_screen_height_ = UINT(HIWORD(lParam));
            m_stage_.OnResize();
        }
        break;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
        // 마우스 시점 변화에 따른 명령
    case WM_MOUSEMOVE:
        command_manager_.OnMouseMove(delta_time, wParam, lParam);
        break;

    case WM_KEYUP:
    case WM_KEYDOWN:
        switch (wParam)
        {
        case(KEY_W):
            command_manager_.OnWKeyUp(delta_time, wParam, lParam);
            break;
        case(KEY_A):
            command_manager_.OnAKeyUp(delta_time, wParam, lParam);
            break;
        case(KEY_S):
            command_manager_.OnSKeyUp(delta_time, wParam, lParam);
            break;
        case(KEY_D):
            command_manager_.OnDKeyUp(delta_time, wParam, lParam);
            break;
        }
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    m_game_loop_.UpdateTimePoint("Message");
    return ::DefWindowProc(hwnd, msg, wParam, lParam);
}


float GraphicsPortfolio1App::GetAspectRatio()
{
    return float(m_screen_width_ - m_imgui_manager.GetImGuiWidth()) / m_screen_height_;
}

int GraphicsPortfolio1App::Run()
{
    MSG msg = { 0 };

    m_game_loop_.AddTimePoint("Main");

    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {;
            const float& delta_time = m_game_loop_.GetElapsed("Main");
            m_game_loop_.UpdateTimePoint("Main");
            m_imgui_manager.RecordRendering(delta_time);

            m_stage_.Update();

            m_stage_.Render();

            m_imgui_manager.Render();

            m_stage_.m_swap_chain_->Present(1, 0);
        }
    }
    return 0;
}

void GraphicsPortfolio1App::SetImGuiDXDelegate()
{
    m_imgui_manager.m_on_file_added_.Add(&m_stage_, &Stage::AddModel);
    m_imgui_manager.m_on_file_deleted_.Add(&m_stage_, &Stage::RemoveModel);
}
