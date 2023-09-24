#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <vector>
#include <string>

#include "LightManageGui.h"
#include "LightManager.h"
#include "DefineVar.h"

using namespace ImGui;
using namespace std;

LightManageGui::LightManageGui(std::shared_ptr<LightManager>& spLightManagerIn)
	: spLightManager(spLightManagerIn),
	bLightTypeCheckFlag{ false, false, false },
	pItems{ "Directional", "Point", "Spot" },
	iSelectedIdx(0)
{
}

LightManageGui::~LightManageGui()
{
}

void LightManageGui::RenderGui()
{
	Begin("Light Mananger");

	if (CollapsingHeader("Add Light"))
	{
		SetLightAddMenu();
	}

	if (CollapsingHeader("Select Light"))
	{
		SetLightSelectorMenu();
	}

	if (CollapsingHeader("Setting"))
	{
		SetLightSettingMenu();
	}
	End();
}

void LightManageGui::SetLightAddMenu()
{
	if (ImGui::BeginCombo("Select Light Type", pItems[iSelectedIdx], NULL))
	{
		for (int n = 0; n < IM_ARRAYSIZE(pItems); n++)
		{
			const bool is_selected = (iSelectedIdx == n);
			if (ImGui::Selectable(pItems[n], is_selected))
				iSelectedIdx = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (iSelectedIdx == LightType::Directional)	SetDirectionalLightMenu(&LightManager::sTempLightSet);
	else if (iSelectedIdx == LightType::Point)	SetPointLightMenu(&LightManager::sTempLightSet);
	else if (iSelectedIdx == LightType::Spot)	SetSpotLightMenu(&LightManager::sTempLightSet);
	else;

	const float& fWindowSize = ImGui::GetWindowWidth();
	if (ImGui::Button("Add", ImVec2(fWindowSize / 2.f, 0.f)))
	{
		switch (iSelectedIdx)
		{
		case LightType::Directional:
			spLightManager->AddDirectionalLight(
				LightManager::sTempLightSet.xmvLocation,
				LightManager::sTempLightSet.xmvLightColor,
				LightManager::sTempLightSet.xmvDirection
			);
			break;
		case LightType::Point:
			break;
		case LightType::Spot:
			break;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset", ImVec2(fWindowSize / 2.f, 0.f)))
	{
		AutoZeroMemory(LightManager::sTempLightSet);
	}
}

void LightManageGui::SetLightSelectorMenu()
{
	if (ImGui::BeginTable("Light Selector", 3, ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Check");
		ImGui::TableSetupColumn("Light ID");
		ImGui::TableSetupColumn("Light Type");
		ImGui::TableHeadersRow();

		const vector<LightSet>& vLights = spLightManager->GetLights();
		const unsigned short& usSelectedLightIndex = spLightManager->GetSelectedLightIndex();
		for (size_t idx = 0; idx < vLights.size(); ++idx)
		{
			bool IsSelected = ((usSelectedLightIndex - 1) == idx);
			ImGui::TableNextRow();
			ImGui::PushID(&vLights[idx]);
			ImGui::TableSetColumnIndex(0);
			ImGui::Checkbox(("Light" + to_string(idx + 1)).c_str(), &IsSelected);

			if (IsSelected)
			{
				spLightManager->SetSelectedLightIndex(idx + 1);
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", idx + 1);
			ImGui::TableSetColumnIndex(2);
			string strLightType;
			switch (vLights[idx].eLightType)
			{
			case LightType::Directional:
				strLightType = "Directional";
				break;
			case LightType::Point:
				strLightType = "Point";
				break;
			case LightType::Spot:
				strLightType = "Spot";
				break;
			case LightType::NotALight:
			default:
				break;
			}
			ImGui::Text(strLightType.c_str());
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}

void LightManageGui::SetLightSettingMenu()
{
	const vector<LightSet>& vLights = spLightManager->GetLights();
	unsigned short usSelectedLighIndex = spLightManager->GetSelectedLightIndex();
	bool bLightNotSelected = ((vLights.size() == 0) || (usSelectedLighIndex == 0));
	if (!bLightNotSelected)
	{
		usSelectedLighIndex = usSelectedLighIndex - 1;
		LightType eLightType = vLights[usSelectedLighIndex].eLightType;
		LightSet* pLightSet = const_cast<LightSet*>(&vLights[usSelectedLighIndex]);
		if (eLightType == LightType::Directional)	SetDirectionalLightMenu(pLightSet);
		else if (eLightType == LightType::Point)	SetPointLightMenu(pLightSet);
		else if (eLightType == LightType::Spot)		SetSpotLightMenu(pLightSet);
		else;
	}
	else
	{
		ImGui::BulletText("Select The Light For the Setting");
	}
}

void LightManageGui::SetDirectionalLightMenu(LightSet* pLightSet)
{
	ImGui::PushID(pLightSet);
	ImGui::SliderFloat3("Light Location", pLightSet->xmvLocation.m128_f32, -10.f, 10.f);
	ImGui::SliderFloat3("Light Color", pLightSet->xmvLightColor.m128_f32, 0, 1.f);
	ImGui::SliderFloat3("Light Direction", pLightSet->xmvDirection.m128_f32, -1.f, 1.f);
	ImGui::SliderFloat("Light Power", &pLightSet->fLightStrength, 0.f, 10.f);
	ImGui::PopID();
}

void LightManageGui::SetPointLightMenu(LightSet* pLightSet)
{
	ImGui::PushID(pLightSet);
	ImGui::SliderFloat3("Light Location", pLightSet->xmvLocation.m128_f32, -100.f, 100.f);
	ImGui::SliderFloat3("Light Color", pLightSet->xmvLightColor.m128_f32, 0, 1.f);
	ImGui::SliderFloat("Light Power", &pLightSet->fLightStrength, 0.f, 10.f);
	ImGui::SliderFloat2("Fall Off Start/End", &pLightSet->fFallOffStart, 0.f, 100.f);
	ImGui::PopID();
}

void LightManageGui::SetSpotLightMenu(LightSet* pLightSet)
{
	ImGui::PushID(pLightSet);
	ImGui::SliderFloat3("Light Location", pLightSet->xmvLocation.m128_f32, -10.f, 10.f);
	ImGui::SliderFloat3("Light Color", pLightSet->xmvLightColor.m128_f32, 0, 1.f);
	ImGui::SliderFloat3("Light Direction", pLightSet->xmvDirection.m128_f32, -1.f, 1.f);
	ImGui::SliderFloat("Light Power", &pLightSet->fLightStrength, 0.f, 10.f);
	ImGui::SliderFloat("Spot Power", &pLightSet->fSpotPower, 0.f, 10.f);
	ImGui::PopID();
}
