#include "CharacterMoveCom.h"
#include <iostream>

using std::cout;
using std::endl;

/*
lParam狼 HIWORD(lParam)啊
0x00?? 老 版快 keydown
0x40?? 老 版快 key keep down
0xC0?? 老 版快 keyup
*/

MoveForwardCom::MoveForwardCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveForwardCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "Move Forward : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

MoveBackWardCom::MoveBackWardCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveBackWardCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "Move Backward : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

MoveRightCom::MoveRightCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveRightCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "Move Right : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}

MoveLeftCom::MoveLeftCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveLeftCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	cout << "Move Left : " << LOWORD(wParam) << "/" << HIWORD(wParam) << " // " << LOWORD(lParam) << "/" << HIWORD(lParam) << '\n';
}
