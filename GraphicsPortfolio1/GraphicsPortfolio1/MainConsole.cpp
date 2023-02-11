#include <iostream>

#include "GraphicsPortfolio1App.h"

using std::cout;
using std::endl;

int main()
{
	GraphicsPortfolio1App App;
	if (!App.Initialize())
	{
		cout << "Application Initialization Failed" << endl;
	}
	
	return App.Run();
}