#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <vector>
#include <string>

#include "LightManageGui.h"
#include "LightManager.h"
#include "LightInterface.h"
#include "DefineVar.h"

using namespace ImGui;
using namespace std;

LightManageGui::LightManageGui(LightManager* pLightManagerIn)
	: pLightManager(pLightManagerIn),
	bLightTypeCheckFlag{ false, false },
	pItems{ "Point", "Spot" },
	iSelectedIdx(0)
{
	pTempLightSet = new LightSet();
	AutoZeroMemory(*pTempLightSet);
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
			{
				iSelectedIdx = n + 1;
			}
			else;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
			else;
		}
		ImGui::EndCombo();
	}

	if (iSelectedIdx == ELightType::PointLightType)	SetPointLightMenu();
	else if (iSelectedIdx == ELightType::SpotLightType)	SetSpotLightMenu();
	else;

	const float& fWindowSize = ImGui::GetWindowWidth();
	if (ImGui::Button("Add", ImVec2(fWindowSize / 2.f, 0.f)))
	{
		switch (iSelectedIdx)
		{
		case ELightType::PointLightType:
			pLightManager->AddPointLight(
				pTempLightSet->xmvLocation,
				pTempLightSet->fLightColor,
				pTempLightSet->fFallOffStart,
				pTempLightSet->fFallOffEnd,
				pTempLightSet->fLightPower
			);
			break;
		case ELightType::SpotLightType:
			pLightManager->AddSpotLight(
				pTempLightSet->xmvLocation,
				pTempLightSet->xmvDirect,
				pTempLightSet->fLightColor,
				pTempLightSet->fFallOffStart,
				pTempLightSet->fFallOffEnd,
				pTempLightSet->fLightPower,
				pTempLightSet->fSpotPower
			);
			break;
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("Reset", ImVec2(fWindowSize / 2.f, 0.f)))
	{
		AutoZeroMemory(*pTempLightSet);
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

		const vector<LightInterface*>& vLights = pLightManager->GetLights();
		const size_t& idxSelectedLight = pLightManager->GetSelectedLightIndex();
		for (size_t idx = 0; idx < vLights.size(); ++idx)
		{
			bool IsSelected = ((idxSelectedLight - 1) == idx);
			ImGui::TableNextRow();
			ImGui::PushID(&vLights[idx]);
			ImGui::TableSetColumnIndex(0);
			ImGui::Checkbox(("Light" + to_string(idx + 1)).c_str(), &IsSelected);

			if (IsSelected)
			{
				pLightManager->SetSelectedLightIndex(idx + 1);
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", idx + 1);
			ImGui::TableSetColumnIndex(2);
			string strLightType;
			switch (vLights[idx]->sBaseLightData.uiLightType)
			{
			case ELightType::PointLightType:
				strLightType = "Point";
				break;
			case ELightType::SpotLightType:
				strLightType = "Spot";
				break;
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
	const vector<LightInterface*>& vLights = pLightManager->GetLights();
	size_t idxSelectedLigh = pLightManager->GetSelectedLightIndex();
	bool bLightNotSelected = ((vLights.size() == 0) || (idxSelectedLigh == 0));
	if (!bLightNotSelected)
	{
		idxSelectedLigh = idxSelectedLigh - 1;
		const uint32_t& uiLightType = vLights[idxSelectedLigh]->sBaseLightData.uiLightType;
		LightInterface* pLight = vLights[idxSelectedLigh];

		if (uiLightType == ELightType::PointLightType)			SetPointLightMenu();
		else if (uiLightType == ELightType::SpotLightType)		SetSpotLightMenu();
		else;
	}
	else
	{
		ImGui::BulletText("Select The Light For the Setting");
	}
}

void LightManageGui::SetPointLightMenu()
{
	ImGui::PushID("Point Light");
	ImGui::SliderFloat3("Light Location", pTempLightSet->xmvLocation.m128_f32, -100.f, 100.f);
	ImGui::SliderFloat3("Light Color", pTempLightSet->fLightColor, 0, 1.f);
	ImGui::SliderFloat2("Fall Off Start/End", &pTempLightSet->fFallOffStart, 0.f, 100.f);
	ImGui::SliderFloat("Light Power", &pTempLightSet->fLightPower, 1.f, 10.f);
	ImGui::PopID();
}

void LightManageGui::SetSpotLightMenu()
{
	ImGui::PushID("Spot Light");
	ImGui::SliderFloat3("Light Location", pTempLightSet->xmvLocation.m128_f32, -100.f, 100.f);
	ImGui::SliderFloat3("Light Direction", pTempLightSet->xmvDirect.m128_f32, -100.f, 100.f);
	ImGui::SliderFloat3("Light Color", pTempLightSet->fLightColor, 0, 1.f);
	ImGui::SliderFloat2("Fall Off Start/End", &pTempLightSet->fFallOffStart, 0.f, 100.f);
	ImGui::SliderFloat("Light Power", &pTempLightSet->fLightPower, 1.f, 10.f);
	ImGui::SliderFloat("Spot Power", &pTempLightSet->fSpotPower, 1.f, 10.f);
	ImGui::PopID();
}
