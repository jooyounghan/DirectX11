#pragma once
#include "ICommand.h"

class MoveForwardCom : public ICommand
{
public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveBackWardCom : public ICommand
{
public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveRightCom : public ICommand
{
public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};

class MoveLeftCom : public ICommand
{
public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};
