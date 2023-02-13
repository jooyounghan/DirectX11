#include "MouseMoveCom.h"
#include <iostream>

using std::cout;
using std::endl;

void MouseMoveCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "delta time : " << delta_time << endl;
	cout << "Mouse Move : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}
