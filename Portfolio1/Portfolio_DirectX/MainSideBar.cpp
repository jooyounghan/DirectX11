#include "MainSideBar.h"
#include "IGuiMenu.h"

#include "ModelManipulator.h"

#include "AStaticMesh.h"

using namespace ImGui;

MainSideBar::MainSideBar() {}

MainSideBar::~MainSideBar() {}

void MainSideBar::InitModelManipulator(AStaticMesh** ppSelectedStaticMeshIn)
{
	vGUIs.emplace_back(new ModelManipulator(ppSelectedStaticMeshIn));
}

void MainSideBar::Draw()
{
	Begin("Main Side Bar");

	ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	for (auto& gui : vGUIs)
	{
		if (gui->IsGuiAvailable())
		{
			gui->AttachToBar();
		}
	}
	End();
}
