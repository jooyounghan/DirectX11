#pragma once
#include <memory>
#include "ICommand.h"

#define KEY_W	0x57
#define KEY_S	0x53
#define KEY_A	0x41
#define KEY_D	0x44

#define COMMAND(Commander, wParam, lParam) if (Commander) Commander->Command(wParam, lParam);

using std::shared_ptr;

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

public:
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	void OnWKeyDown(WPARAM wParam, LPARAM lParam);
	void OnAKeyDown(WPARAM wParam, LPARAM lParam);
	void OnSKeyDown(WPARAM wParam, LPARAM lParam);
	void OnDKeyDown(WPARAM wParam, LPARAM lParam);

public:
	void SetDefaultCommand();
};

