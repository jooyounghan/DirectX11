#pragma once
#include <mutex>
#include <string>

class Console
{
public:
	static std::mutex mtxConsole;
	static void Print(const std::string& str);
};

