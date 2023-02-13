#pragma once
#include "ICommand.h"

class MouseMoveCom : public ICommand
{
public:
	virtual void Command(const float& delta_time, WPARAM wParam, LPARAM lParam);
};

