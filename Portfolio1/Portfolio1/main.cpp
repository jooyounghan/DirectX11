#include <iostream>
#include <mutex>

#include "PortfolioApp.h"

std::mutex mtxConsole;
void Console(const char* text)
{
    std::lock_guard<std::mutex> lockGuard(mtxConsole);
    std::cout << text << std::endl;
}

int main() {
    UINT iWidth = 1280;
    UINT iHeight = 960;
    PortfolioApp* App = new PortfolioApp(iWidth, iHeight);

    App->Init();
    App->Run();
    App->Quit();

    delete App;
    return 0;
}