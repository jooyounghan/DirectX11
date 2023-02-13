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

void CommandManager::OnMouseMove(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	mouse_move_command_->Command(delta_time, wParam, lParam);
}

void CommandManager::OnWKeyDown(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	wbutton_up_command_->Command(delta_time, wParam, lParam);
}

void CommandManager::OnAKeyDown(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	sbutton_up_command_->Command(delta_time, wParam, lParam);
}

void CommandManager::OnSKeyDown(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	abutton_up_command_->Command(delta_time, wParam, lParam);
}

void CommandManager::OnDKeyDown(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	dbutton_up_command_->Command(delta_time, wParam, lParam);
}

void CommandManager::SetDefaultCommand()
{
	mouse_move_command_ = make_shared<MouseMoveCom>();

	wbutton_up_command_ = make_shared<MoveForwardCom>();
	abutton_up_command_ = make_shared<MoveLeftCom>();
	sbutton_up_command_ = make_shared<MoveBackWardCom>();
	dbutton_up_command_ = make_shared<MoveRightCom>();
}
