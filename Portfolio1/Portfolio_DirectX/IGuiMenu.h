#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

class IGuiMenu
{
public:
	IGuiMenu() {};
	~IGuiMenu() {};

public:
	virtual void AttachToBar() = 0;
	virtual bool IsGuiAvailable() = 0;
};

