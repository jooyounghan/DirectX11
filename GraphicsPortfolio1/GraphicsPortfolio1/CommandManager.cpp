#include "CommandManager.h"

#include "CharacterMoveCom.h"
#include "CameraRotCom.h"

using std::make_shared;

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
}

void CommandManager::OnMouseMove(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	mouse_move_command_->Command(hwnd, delta_time, wParam, lParam);
}

void CommandManager::OnWKeyUp(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	wbutton_up_command_->Command(hwnd, delta_time, wParam, lParam);
}

void CommandManager::OnAKeyUp(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	abutton_up_command_->Command(hwnd, delta_time, wParam, lParam);
}

void CommandManager::OnSKeyUp(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	sbutton_up_command_->Command(hwnd, delta_time, wParam, lParam);
}

void CommandManager::OnDKeyUp(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	dbutton_up_command_->Command(hwnd, delta_time, wParam, lParam);
}


void CommandManager::SetDefaultCommand(shared_ptr<Camera> camera)
{
	mouse_move_command_ = make_shared<CameraRotCom>(camera);

	wbutton_up_command_ = make_shared<MoveForwardCom>(camera);
	abutton_up_command_ = make_shared<MoveLeftCom>(camera);
	sbutton_up_command_ = make_shared<MoveBackWardCom>(camera);
	dbutton_up_command_ = make_shared<MoveRightCom>(camera);
}
