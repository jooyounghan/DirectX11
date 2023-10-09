#include "GroupManageGui.h"
#include "GroupManageGui.h"

GroupManageGui::GroupManageGui()
{
}

GroupManageGui::~GroupManageGui()
{
}

void GroupManageGui::RenderGui()
{
	for (auto& upGui : upGuis)
	{
		upGui->RenderGui();
	}
}

void GroupManageGui::AddGui(const std::unique_ptr<ManageGuiInterface>& upGui)
{
	upGuis.push_back(upGui);
}
