#include "MouseMoveCom.h"
#include <iostream>

using std::cout;
using std::endl;

void MouseMoveCom::Command(WPARAM wParam, LPARAM lParam)
{
	cout << "Mouse Move : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}
