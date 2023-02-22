#include "CharacterMoveCom.h"
#include <iostream>

using std::cout;
using std::endl;

MoveForwardCom::MoveForwardCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveForwardCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	if (GET_CLICK_STATE(lParam,ON_CLICKSTART))
	{
		m_camera_->SetOnCameraMoveFlag(CAMERA_MOVE_FORWARD);
	}
	else if (GET_CLICK_STATE(lParam, ON_CLICKED))
	{

	}
	else if (GET_CLICK_STATE(lParam, ON_CLICKFINISH))
	{
		m_camera_->SetOffCameraMoveFlag(CAMERA_MOVE_FORWARD);
	}
}

MoveBackWardCom::MoveBackWardCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveBackWardCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	if (GET_CLICK_STATE(lParam, ON_CLICKSTART))
	{
		m_camera_->SetOnCameraMoveFlag(CAMERA_MOVE_BACKWARD);
	}
	else if (GET_CLICK_STATE(lParam, ON_CLICKED))
	{

	}
	else if (GET_CLICK_STATE(lParam, ON_CLICKFINISH))
	{
		m_camera_->SetOffCameraMoveFlag(CAMERA_MOVE_BACKWARD);
	}
}

MoveRightCom::MoveRightCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveRightCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	if (GET_CLICK_STATE(lParam, ON_CLICKSTART))
	{
		m_camera_->SetOnCameraMoveFlag(CAMERA_MOVE_RIGHT);
	}
	else if (GET_CLICK_STATE(lParam, ON_CLICKED))
	{

	}
	else if (GET_CLICK_STATE(lParam, ON_CLICKFINISH))
	{
		m_camera_->SetOffCameraMoveFlag(CAMERA_MOVE_RIGHT);
	}
}

MoveLeftCom::MoveLeftCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveLeftCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{

	if (GET_CLICK_STATE(lParam, ON_CLICKSTART))
	{
		m_camera_->SetOnCameraMoveFlag(CAMERA_MOVE_LEFT);
	}
	else if (GET_CLICK_STATE(lParam, ON_CLICKED))
	{

	}
	else if (GET_CLICK_STATE(lParam, ON_CLICKFINISH))
	{
		m_camera_->SetOffCameraMoveFlag(CAMERA_MOVE_LEFT);
	}
}
