#include "CharacterMoveCom.h"
#include <iostream>

using std::cout;
using std::endl;

void MoveForwardCom::Command(WPARAM wParam, LPARAM lParam)
{
	cout << "Move Forward : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

void MoveBackWardCom::Command(WPARAM wParam, LPARAM lParam)
{
	cout << "Move Backward : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

void MoveRightCom::Command(WPARAM wParam, LPARAM lParam)
{
	cout << "Move Right : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

void MoveLeftCom::Command(WPARAM wParam, LPARAM lParam)
{
	cout << "Move Left : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}
