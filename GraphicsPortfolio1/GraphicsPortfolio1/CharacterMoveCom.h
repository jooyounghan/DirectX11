#pragma once

#include "ICommand.h"
#include "Camera.h"

class MoveForwardCom : public ICommand
{
public:
	MoveForwardCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveBackWardCom : public ICommand
{
public:
	MoveBackWardCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveRightCom : public ICommand
{
public:
	MoveRightCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveLeftCom : public ICommand
{
public:
	MoveLeftCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};
