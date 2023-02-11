#pragma once
#include <Windows.h>

class ICommand
{
public:
	virtual void Command(WPARAM wParam, LPARAM lParam) = 0;
};

