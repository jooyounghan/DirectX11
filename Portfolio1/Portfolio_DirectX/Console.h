#pragma once
#include <mutex>
#include <string>

class Console
{
private:
	static std::mutex mtxConsole;

public:
	static void Print(const std::string& str);
};

