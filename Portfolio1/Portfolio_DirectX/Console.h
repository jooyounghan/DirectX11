#pragma once
#include <mutex>
#include <string>

class Console
{
private:
	static std::mutex mtxConsole;

public:
	static void Print(const std::string& str);
	static void AssertPrint(const std::string& str);
};

