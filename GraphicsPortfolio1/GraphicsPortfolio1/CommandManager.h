#pragma once
#include "ICommand.h"

#define KEY_W	0x57
#define KEY_S	0x53
#define KEY_A	0x41
#define KEY_D	0x44

#define COMMAND(Commander, wParam, lParam) if (Commander) Commander->Command(wParam, lParam);


class CommandManager
{
public:
	CommandManager();
	~CommandManager();

public:
	shared_ptr<ICommand> mouse_move_command_ = nullptr;

	shared_ptr<ICommand> wbutton_up_command_ = nullptr;
	shared_ptr<ICommand> abutton_up_command_ = nullptr;
	shared_ptr<ICommand> sbutton_up_command_ = nullptr;
	shared_ptr<ICommand> dbutton_up_command_ = nullptr;

	shared_ptr<ICommand> wbutton_down_command_ = nullptr;
	shared_ptr<ICommand> abutton_down_command_ = nullptr;
	shared_ptr<ICommand> sbutton_down_command_ = nullptr;
	shared_ptr<ICommand> dbutton_down_command_ = nullptr;

public:
	void OnMouseMove(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);

	void OnWKeyUp(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);
	void OnAKeyUp(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);
	void OnSKeyUp(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);
	void OnDKeyUp(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);

public:
	void SetDefaultCommand(shared_ptr<class Camera> camera);
};

