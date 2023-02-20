#pragma once

#include <memory>
#include <Windows.h>

using std::shared_ptr;

class ICommand
{
public:
	virtual void Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam) = 0;
};

