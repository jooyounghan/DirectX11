#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "SettingManageGui.h"
#include "CubeMapModel.h"

#include "DDSFile.h"
#include "ModelTextureFile.h"

#include "RasterizationState.h"

using namespace std;
using namespace ImGui;

SettingManageGui::SettingManageGui(
	ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn,
	bool& bIsNormalVectorDrawIn, bool& bIsWireFrameDrawIn, 
	shared_ptr<class CubeMapModel> spCubeMapModelIn
)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn), 
	bIsNormalVectorDraw(bIsNormalVectorDrawIn), bIsWireFrameDraw(bIsWireFrameDrawIn), 
	spCubeMapModel(spCubeMapModelIn)
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
	bool lastFlag = bIsWireFrameDraw;
	ImGui::Checkbox("Draw WireFrame", &bIsWireFrameDraw);
	if (lastFlag != bIsWireFrameDraw)
	{
		lastFlag = bIsWireFrameDraw;
		RasterizationState& rasterizerState = RasterizationState::GetInstance(pDevice, pDeviceContext);
		if (bIsWireFrameDraw)
		{
			rasterizerState.SetWireFrameRS();
		}
		else
		{
			rasterizerState.SetSolidRS();
		}
	}
}

void SettingManageGui::SetCubeMapTexture()
{
	Separator();
	SetDDSTexture("Environment Specular", spCubeMapModel->spEnvSpecularTextureFile);
	SetDDSTexture("Environment Diffuse", spCubeMapModel->spEnvDiffuseTextureFile);
	SetModelTexture("Environment BRDF", spCubeMapModel->spEnvBrdfTextureFile);
}

void SettingManageGui::SetDDSTexture(const string& strDescription, shared_ptr<DDSFile>& spDDSFile)
{
	Separator();
	Text(strDescription.c_str());
	BeginGroup();
	if (spDDSFile != nullptr)
	{
		Image(spDDSFile->cpFileThumbNailSRV.Get(), ImVec2(60.f, 60.f));
		SameLine();
		Text(spDDSFile->strFileName.c_str());
	}
	else
	{
		Image(nullptr, ImVec2(60.f, 60.f));
		SameLine();
		Text("");
	}
	EndGroup();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CubeMap"))
		{
			spDDSFile = *(shared_ptr<DDSFile>*)payload->Data;
		}
		ImGui::EndDragDropTarget();
	}
}

void SettingManageGui::SetModelTexture(const std::string& strDescription, shared_ptr<ModelTextureFile>& spModelTextureFile)
{
	Separator();
	Text(strDescription.c_str());
	BeginGroup();
	if (spModelTextureFile != nullptr)
	{
		Image(spModelTextureFile->cpFileThumbNailSRV.Get(), ImVec2(60.f, 60.f));
		SameLine();
		Text(spModelTextureFile->strFileName.c_str());
	}
	else
	{
		Image(nullptr, ImVec2(60.f, 60.f));
		SameLine();
		Text("");
	}
	EndGroup();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture2D"))
		{
			spModelTextureFile = *(shared_ptr<ModelTextureFile>*)payload->Data;
		}
		ImGui::EndDragDropTarget();
	}
}
