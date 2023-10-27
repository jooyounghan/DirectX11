#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <directxmath/DirectXMath.h>

#include "ModelManageGui.h"
#include "ModelInterface.h"

#include "FileLoader.h"
#include "ModelTextureLoader.h"

using namespace ImGui;
using namespace DirectX;
using namespace std;

ModelManageGui::ModelManageGui(
	std::vector<std::shared_ptr<ModelInterface>>& vSpModelsIn,
	std::shared_ptr<ModelInterface>& spSelectedModelIn,
	std::shared_ptr<ModelInterface>& spTempSelectedModelIn
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
	SliderFloat3("Scale Vector",  spSelectedModel->sTransformationProperties.xmvScale.m128_f32, 0.f, 5.f);
	SliderFloat3("Rotation Vector", (float*)(&spSelectedModel->sTransformationProperties.sPositionAngle), -2.f * XM_PI, 2.f * XM_PI);
	SliderFloat3("Translation Vector", spSelectedModel->sTransformationProperties.xmvTranslation.m128_f32, -10.f, 10.f);
}

void ModelManageGui::SetModelTexture()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::BeginChild("ModelTextureSet", ImGui::GetContentRegionAvail(), false, window_flags);
	//for (unsigned short idx = 0; idx < MODEL_TEXTURE::MODEL_TEXTURE_NUM; ++idx)
	//{
	//	Separator();
	//	Text(ModelTexture::strTextureType[idx].c_str());
	//	Image(spSelectedModel->pModelTextureSet->GetSRV((MODEL_TEXTURE)idx).Get(), ImVec2(60.f, 60.f));

	//	SameLine();
	//	Text(FileLoader::ConvertWCharToChar(spSelectedModel->pModelTextureSet->GetName((MODEL_TEXTURE)idx)).c_str());
	//}
	ImGui::EndChild();
}

