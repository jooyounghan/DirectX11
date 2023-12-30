#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <vector>
#include <string>

#include "LightManageGui.h"
#include "LightManager.h"
#include "LightInterface.h"

#include "PointLight.h"
#include "SpotLight.h"

#include "DefineVar.h"

using namespace ImGui;
using namespace std;

LightManageGui::LightManageGui(LightManager* pLightManagerIn)
	: pLightManager(pLightManagerIn),
	bLightTypeCheckFlag{ false, false },
	pItems{ "Point", "Spot" },
	uiSelLightType(0), uiSelTabIdx(0)
{
	pTempPointLightSet = new PointLightSet();
	pTempSpotLightSet = new SpotLightSet();
	AutoZeroMemory(*pTempPointLightSet);
	pTempPointLightSet->fLightPower = 1.f;
	AutoZeroMemory(*pTempSpotLightSet);
	pTempSpotLightSet->fLightPower = 1.f;
	pTempSpotLightSet->fSpotPower = 1.f;
}

LightManageGui::~LightManageGui()
{
	delete pTempPointLightSet;
	delete pTempSpotLightSet;
	pTempPointLightSet = nullptr;
	pTempSpotLightSet = nullptr;
}

void LightManageGui::RenderGui()
{
	Begin("Light Mananger");

	if (CollapsingHeader("Add Light"))
	{
		SetLightAddMenu();
	}
	Separator();

	if (CollapsingHeader("Select Light"))
	{
		SetLightSelectorTab();
	}
	Separator();
	
	if (CollapsingHeader("Setting"))
	{
		SetLightSettingMenu();
	}

	End();
}

void LightManageGui::SetLightAddMenu()
{
	Text("Select Light Type");
	if (ImGui::BeginCombo(" ", pItems[uiSelLightType], NULL))
	{
		for (UINT n = 0; n < IM_ARRAYSIZE(pItems); n++)
		{
			const bool is_selected = (uiSelLightType == n);
			if (ImGui::Selectable(pItems[n], is_selected))
			{
				uiSelLightType = n;
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

	switch (uiSelLightType)
	{
	case ELightType::PointLightType:
		SetPointLightMenu(pTempPointLightSet);
		break;
	case ELightType::SpotLightType:
		SetSpotLightMenu(pTempSpotLightSet);
	default:
		break;
	}

	const float& fWindowSize = ImGui::GetWindowWidth();
	if (ImGui::Button("Add", ImVec2(fWindowSize / 2.f, 0.f)))
	{
		switch (uiSelLightType)
		{
		case ELightType::PointLightType:
			pLightManager->AddPointLight(
				pTempPointLightSet->xmvLocation,
				pTempPointLightSet->xmvLightColor,
				pTempPointLightSet->fFallOffStart,
				pTempPointLightSet->fFallOffEnd,
				pTempPointLightSet->fLightPower
			);
			AutoZeroMemory(*pTempPointLightSet);
			pTempPointLightSet->fLightPower = 1.f;
			break;
		case ELightType::SpotLightType:
			pLightManager->AddSpotLight(
				pTempSpotLightSet->xmvLocation,
				pTempSpotLightSet->xmvAngles,
				pTempSpotLightSet->xmvLightColor,
				pTempSpotLightSet->fFallOffStart,
				pTempSpotLightSet->fFallOffEnd,
				pTempSpotLightSet->fLightPower,
				pTempSpotLightSet->fSpotPower
			);
			AutoZeroMemory(*pTempSpotLightSet);
			pTempSpotLightSet->fLightPower = 1.f;
			pTempSpotLightSet->fSpotPower = 1.f;
			break;
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("Reset", ImVec2(fWindowSize / 2.f, 0.f)))
	{
		AutoZeroMemory(*pTempPointLightSet);
		AutoZeroMemory(*pTempSpotLightSet);
	}
}

template<typename Ptr>
void LightManageGui::SetLightSelector(const char* label, const std::vector<Ptr*>& vLightsIn, const size_t& idxSel, size_t& idxSelected)
{
	if (ImGui::BeginTable(label, 3, ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Check");
		ImGui::TableSetupColumn("Light ID");
		ImGui::TableHeadersRow();

		for (size_t idx = 0; idx < vLightsIn.size(); ++idx)
		{
			bool IsSelected = ((idxSel) == idx);
			ImGui::TableNextRow();
			ImGui::PushID(&vLightsIn[idx]);
			ImGui::TableSetColumnIndex(0);
			ImGui::Checkbox((label + to_string(idx + 1)).c_str(), &IsSelected);

			if (IsSelected)
			{
				idxSelected = idx;
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", idx + 1);
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}


void LightManageGui::SetLightSelectorTab()
{
	if (ImGui::BeginTabBar("Light Tab Bar", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Point"))
		{
			const vector<PointLight*> vPoints = pLightManager->GetPointLights();
			SetLightSelector("Point Light", vPoints, pLightManager->GetSelPointLightIndex(), pLightManager->GetSelPointLightIndexRef());
			uiSelTabIdx = ELightType::PointLightType;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Spot"))
		{
			const vector<SpotLight*> vPoints = pLightManager->GetSpotLights();
			SetLightSelector("Spot Light", vPoints, pLightManager->GetSelSpotLightIndex(), pLightManager->GetSelSpotLightIndexRef());
			uiSelTabIdx = ELightType::SpotLightType;
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void LightManageGui::SetLightSettingMenu()
{
	bool bLightSelected = false;

	if (uiSelTabIdx == ELightType::PointLightType)
	{
		const vector<PointLight*>& vPoints = pLightManager->GetPointLights();
		const size_t& idxSelectedLight = pLightManager->GetSelPointLightIndex();
		bLightSelected = (vPoints.size() != 0);
		if (bLightSelected)
		{
			SetPointLightMenu(&reinterpret_cast<PointLight*>(vPoints[idxSelectedLight])->sPointLightSet);
		}
		else
		{
			ImGui::BulletText("Select The Light For the Setting");
		}
	}
	else if (uiSelTabIdx == ELightType::SpotLightType)
	{
		const vector<SpotLight*>& vPoints = pLightManager->GetSpotLights();
		const size_t& idxSelectedLight = pLightManager->GetSelSpotLightIndex();
		bLightSelected = (vPoints.size() != 0);
		if (bLightSelected)
		{
			SetSpotLightMenu(&reinterpret_cast<SpotLight*>(vPoints[idxSelectedLight])->sSpotLightSet);
		}
		else 
		{
			ImGui::BulletText("Select The Light For the Setting");
		}
	}
	else;
}

void LightManageGui::SetPointLightMenu(PointLightSet* pPointLightSet)
{
	ImGui::PushID(pPointLightSet);

	ImGui::PushID("Location");
	ImGui::Text("Light Location (X, Y, Z)");
	ImGui::SliderFloat3("", pPointLightSet->xmvLocation.m128_f32, -100.f, 100.f);
	ImGui::PopID();

	ImGui::PushID("Color");
	ImGui::Text("Light Color (R, G, B)");
	ImGui::SliderFloat3("", pPointLightSet->xmvLightColor.m128_f32, 0, 1.f);
	ImGui::PopID();

	ImGui::PushID("Attenuate");
	ImGui::Text("Attenuate Factor (Fall-Off-Start / Fall-Off-End)");
	ImGui::SliderFloat2("", &pPointLightSet->fFallOffStart, 0.f, 100.f);
	ImGui::PopID();

	ImGui::PushID("LPower");
	ImGui::Text("Light Power");
	ImGui::SliderFloat("", &pPointLightSet->fLightPower, 1.f, 10.f);
	ImGui::PopID();

	ImGui::PopID();
}

void LightManageGui::SetSpotLightMenu(SpotLightSet* pSpotLightSet)
{
	ImGui::PushID(pSpotLightSet);

	ImGui::PushID("Location");
	ImGui::Text("Light Location (X, Y, Z)");
	ImGui::SliderFloat3("", pSpotLightSet->xmvLocation.m128_f32, -100.f, 100.f);
	ImGui::PopID();

	ImGui::PushID("Angles");
	ImGui::Text("Light Angles(Deg) (Pitch, Yaw, Roll)");
	ImGui::SliderFloat3("", pSpotLightSet->xmvAngles.m128_f32, 0.f, 360.f);
	ImGui::PopID();

	ImGui::PushID("Color");
	ImGui::Text("Light Color (R, G, B)");
	ImGui::SliderFloat3("", pSpotLightSet->xmvLightColor.m128_f32, 0, 1.f);
	ImGui::PopID();

	ImGui::PushID("Attenuate");
	ImGui::Text("Attenuate Factor (Fall-Off-Start / Fall-Off-End)");
	ImGui::SliderFloat2("", &pSpotLightSet->fFallOffStart, 0.f, 100.f);
	ImGui::PopID();

	ImGui::PushID("LPower");
	ImGui::Text("Light Power");
	ImGui::SliderFloat("", &pSpotLightSet->fLightPower, 1.f, 10.f);
	ImGui::PopID();

	ImGui::PushID("SPower");
	ImGui::Text("Spot Power");
	ImGui::SliderFloat("", &pSpotLightSet->fSpotPower, 1.f, 10.f);
	ImGui::PopID();

	ImGui::PopID();
}
