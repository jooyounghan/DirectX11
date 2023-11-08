#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "SettingManageGui.h"
#include "CubeMapModel.h"
#include "DDSFile.h"

using namespace std;
using namespace ImGui;

SettingManageGui::SettingManageGui(bool& bIsNormalVectorDrawIn, bool& bIsWireFrameDrawIn, shared_ptr<class CubeMapModel> spCubeMapModelIn)
	: bIsNormalVectorDraw(bIsNormalVectorDrawIn), bIsWireFrameDraw(bIsWireFrameDrawIn), spCubeMapModel(spCubeMapModelIn)
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
	SetDrawWireFrame();
	SetCubeMapTexture();

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

void SettingManageGui::SetCubeMapTexture()
{
	Separator();
	Text("Cube Map Texture");
	if (spCubeMapModel->pDDSTextureFile != nullptr)
	{
		Image(nullptr, ImVec2(60.f, 60.f));
	}
	else
	{
		Image(nullptr, ImVec2(60.f, 60.f));
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DDSFile"))
		{
			spCubeMapModel->pDDSTextureFile = *(shared_ptr<DDSFile>*)payload->Data;
		}
		ImGui::EndDragDropTarget();
	}

	if (spCubeMapModel->pDDSTextureFile != nullptr)
	{
		SameLine();
		Text(spCubeMapModel->pDDSTextureFile->strFileName.c_str());
	}
	else
	{
		SameLine();
		Text("");
	}
}
