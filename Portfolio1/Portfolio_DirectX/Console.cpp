#include "Console.h"
#include <iostream>

std::mutex Console::mtxConsole;

void Console::Print(const std::string& str)
{
	std::lock_guard<std::mutex> lockGuard(mtxConsole);
	std::cout << str << std::endl;
}
