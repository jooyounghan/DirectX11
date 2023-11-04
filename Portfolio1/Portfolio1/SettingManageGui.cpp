#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "SettingManageGui.h"

using namespace ImGui;

SettingManageGui::SettingManageGui(bool& bIsNormalVectorDrawIn, bool& bIsWireFrameDrawIn)
	: bIsNormalVectorDraw(bIsNormalVectorDrawIn), bIsWireFrameDraw(bIsWireFrameDrawIn)
{
}

SettingManageGui::~SettingManageGui()
{
}

void SettingManageGui::RenderGui()
{
	Begin("Normal Setting");
	Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	SetDrawNormalVector();

	End();
}

void SettingManageGui::SetDrawNormalVector()
{
	ImGui::Checkbox("Draw Normal Vector", &bIsNormalVectorDraw);
}

void SettingManageGui::SetDrawWireFrame()
{
	ImGui::Checkbox("Draw WireFrame", &bIsWireFrameDraw);
}
