#include "CharacterMoveCom.h"
#include <iostream>

using std::cout;
using std::endl;

void MoveForwardCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "Move Forward : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

void MoveBackWardCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "Move Backward : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

void MoveRightCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "Move Right : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

void MoveLeftCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "Move Left : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}
