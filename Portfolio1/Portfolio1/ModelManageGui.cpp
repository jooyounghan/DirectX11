#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <directxmath/DirectXMath.h>

#include "ModelManageGui.h"
#include "TempVariable.h"
#include "ModelInterface.h"

using namespace ImGui;
using namespace DirectX;

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
	Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	if (CollapsingHeader("Add Model"))
	{

	}

	if (CollapsingHeader("Transform Model"))
	{
		SetTransformModelMenu();
	}

	if (CollapsingHeader("Set Texture"))
	{
		SetModelTexture();
	}
	End();
}

void ModelManageGui::SetTransformModelMenu()
{
	bool bModelNotSelected = (spSelectedModel == nullptr);
	BeginDisabled(bModelNotSelected);
	SliderFloat3("Scale Vector", bModelNotSelected ? TempVariable::fTempFloat3 : spSelectedModel->sTransformationProperties.xmvScale.m128_f32, 0.f, 5.f);
	SliderFloat3("Rotation Vector", bModelNotSelected ? TempVariable::fTempFloat3 : (float*)(&spSelectedModel->sTransformationProperties.sPositionAngle), -2.f * XM_PI, 2.f * XM_PI);
	SliderFloat3("Translation Vector", bModelNotSelected ? TempVariable::fTempFloat3 : spSelectedModel->sTransformationProperties.xmvTranslation.m128_f32, -10.f, 10.f);
	EndDisabled();
}

void ModelManageGui::SetModelTexture()
{

}

