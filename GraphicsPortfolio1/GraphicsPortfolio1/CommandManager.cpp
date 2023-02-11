#include "CommandManager.h"

#include "CharacterMoveCom.h"
#include "MouseMoveCom.h"

using std::make_shared;

CommandManager::CommandManager()
{
	SetDefaultCommand();
}

CommandManager::~CommandManager()
{
}

void CommandManager::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	mouse_move_command_->Command(wParam, lParam);
}

void CommandManager::OnWKeyDown(WPARAM wParam, LPARAM lParam)
{
	wbutton_up_command_->Command(wParam, lParam);
}

void CommandManager::OnAKeyDown(WPARAM wParam, LPARAM lParam)
{
	sbutton_up_command_->Command(wParam, lParam);
}

void CommandManager::OnSKeyDown(WPARAM wParam, LPARAM lParam)
{
	abutton_up_command_->Command(wParam, lParam);
}

void CommandManager::OnDKeyDown(WPARAM wParam, LPARAM lParam)
{
	dbutton_up_command_->Command(wParam, lParam);
}

void CommandManager::SetDefaultCommand()
{
	mouse_move_command_ = make_shared<MouseMoveCom>();

	wbutton_up_command_ = make_shared<MoveForwardCom>();
	abutton_up_command_ = make_shared<MoveLeftCom>();
	sbutton_up_command_ = make_shared<MoveBackWardCom>();
	dbutton_up_command_ = make_shared<MoveRightCom>();
}
