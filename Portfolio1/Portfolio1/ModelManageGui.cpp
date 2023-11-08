#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <directxmath/DirectXMath.h>

#include "ModelManageGui.h"
#include "ObjectModel.h"
#include "FileLoader.h"

#include "ModelTextureFile.h"
#include "TransformProperties.h"

using namespace ImGui;
using namespace DirectX;
using namespace std;

ModelManageGui::ModelManageGui(
	std::vector<std::shared_ptr<ObjectModel>>& vSpModelsIn,
	std::shared_ptr<ObjectModel>& spSelectedModelIn,
	std::shared_ptr<ObjectModel>& spTempSelectedModelIn
)
	: vSpModels(vSpModelsIn),
	spSelectedModel(spSelectedModelIn),
	spTempSelectedModel(spTempSelectedModelIn)
{
}

ModelManageGui::~ModelManageGui()
{
}

void ModelManageGui::RenderGui()
{
	Begin("Model Manager");
	if (CollapsingHeader("Add Model"))
	{

	}
	bool bModelNotSelected = (spSelectedModel == nullptr);
	if (!bModelNotSelected)
	{
		if (CollapsingHeader("Transform Model"))
		{
			SetTransformModelMenu();
		}

		if (CollapsingHeader("Set Texture"))
		{
			SetModelTexture();
		}
		
	}
	else
	{
		ImGui::BulletText("Select The Model For the Setting");
	}

	End();
}

void ModelManageGui::SetTransformModelMenu()
{
	SliderFloat3("Scale Vector",  spSelectedModel->upTransformationProperties->xmvScale.m128_f32, 0.f, 5.f);
	SliderFloat3("Rotation Vector", (float*)(&spSelectedModel->upTransformationProperties->sPositionAngle), -2.f * XM_PI, 2.f * XM_PI);
	SliderFloat3("Translation Vector", spSelectedModel->upTransformationProperties->xmvTranslation.m128_f32, -10.f, 10.f);
}

void ModelManageGui::SetModelTexture()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::BeginChild("ModelTextureSet", ImGui::GetContentRegionAvail(), false, window_flags);
	for (unsigned short idx = 0; idx < MODEL_TEXTURE::MODEL_TEXTURE_NUM; ++idx)
	{
		Separator();
		Text(ModelTextureFile::strTextureType[idx].c_str());
		if (spSelectedModel->pModelTextureSet[idx] != nullptr)
		{
			Image(spSelectedModel->pModelTextureSet[idx]->cpFileThumbNailSRV.Get(), ImVec2(60.f, 60.f));
		}
		else
		{
			Image(nullptr, ImVec2(60.f, 60.f));

		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ModelTextureFile"))
			{
				spSelectedModel->pModelTextureSet[(MODEL_TEXTURE)idx] = *(shared_ptr<ModelTextureFile>*)payload->Data;
			}
			ImGui::EndDragDropTarget();
		}

		if (spSelectedModel->pModelTextureSet[idx] != nullptr)
		{
			SameLine();
			Text(spSelectedModel->pModelTextureSet[idx]->strFileName.c_str());
		}
		else
		{
			SameLine();
			Text("");
		}
	}
	ImGui::EndChild();
}

