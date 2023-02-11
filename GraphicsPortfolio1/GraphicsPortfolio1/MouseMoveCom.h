#pragma once
#include "ICommand.h"

class MouseMoveCom : public ICommand
{
public:
	virtual void Command(WPARAM wParam, LPARAM lParam);
};

