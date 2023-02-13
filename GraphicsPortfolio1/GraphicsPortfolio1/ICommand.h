#pragma once
#include <Windows.h>

class ICommand
{
public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam) = 0;
};

