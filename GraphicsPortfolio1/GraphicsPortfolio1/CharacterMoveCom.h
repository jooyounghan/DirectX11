#pragma once

#include "ICommand.h"
#include "Camera.h" 

#define GET_CLICK_STATE(lParam, check_state)	((HIWORD(lParam) & 0xFF00) == check_state)

#define ON_CLICKSTART		0x0000
#define ON_CLICKED			0x4000
#define ON_CLICKFINISH		0xC000

class MoveForwardCom : public ICommand
{
public:
	MoveForwardCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveBackWardCom : public ICommand
{
public:
	MoveBackWardCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveRightCom : public ICommand
{
public:
	MoveRightCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveLeftCom : public ICommand
{
public:
	MoveLeftCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);
};
