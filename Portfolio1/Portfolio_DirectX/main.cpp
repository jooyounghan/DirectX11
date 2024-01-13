#include "PortfolioApp.h"

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