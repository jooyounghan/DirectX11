#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include <vector>

class MainSideBar
{
public:
	MainSideBar();
	~MainSideBar();

public:
	std::vector<class IGuiMenu*> vGUIs;

public:
	void InitModelManipulator(class AStaticMesh** ppSelectedStaticMeshIn);

public:
	void Draw();
};

