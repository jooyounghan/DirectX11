#include "LightManipulator.h"

#include "DefineVar.h"
#include "SpotLight.h"
#include "PointLight.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <DirectXMath.h>

using namespace ImGui;
using namespace std;

const char* LightManipulator::lightKind[] = {"Spot Light", "Point Light"};

LightManipulator::LightManipulator()
	: 
	fFallOffStart(0.f),
	fFallOffEnd(0.f),
	fLightPower(1.f),
	fSpotPower(1.f),
	bIsAddingLight(false),
	iLightSelectedIdx(NOT_SELECTED),
	ullLightListCheckIdx(0),
	ullSelectedLightKind(0)
{
	AutoZeroMemory(fLightPos);
	AutoZeroMemory(fLightAngleDeg);
	AutoZeroMemory(fLightColor);
	fLightColor[0] = 1.f;
	fLightColor[1] = 1.f;
	fLightColor[2] = 1.f;
}

LightManipulator::~LightManipulator()
{
}

void LightManipulator::InitLightVariable()
{
	AutoZeroMemory(fLightPos);
	AutoZeroMemory(fLightAngleDeg);
	AutoZeroMemory(fLightColor);
	fLightColor[0] = 1.f;
	fLightColor[1] = 1.f;
	fLightColor[2] = 1.f;
	fFallOffStart = 0.f;
	fFallOffEnd = 0.f;
	fLightPower = 1.f;
	fSpotPower = 1.f;
}

void LightManipulator::SetLightGui()
{
	if (CollapsingHeader("Light Setting", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		BeginGroup();
		const ImVec2 buttonSize = ImVec2(GetWindowContentRegionWidth() / 2.f, GetTextLineHeight() * 1.1f);
		if (Button("Add Light", buttonSize))
		{
			bIsAddingLight = true;
		}
		if (Button("Remove Light", buttonSize) && iLightSelectedIdx >= 0)
		{
			spSelectedLight = nullptr;
			pLights.erase(pLights.begin() + iLightSelectedIdx);
			iLightSelectedIdx = NOT_SELECTED;
		}
		if (Button("Light Info", buttonSize))
		{
			bIsShowingLightInfo = true;
		}
		EndGroup();

		SameLine();

		if (BeginListBox("Light List", ImVec2(GetWindowContentRegionWidth() / 2.f, 3.f * GetTextLineHeightWithSpacing())))
		{
			for (ullLightListCheckIdx = 0; ullLightListCheckIdx < pLights.size(); ++ullLightListCheckIdx)
			{
				pLights[ullLightListCheckIdx]->AcceptLightList(this);
			}
			EndListBox();
		}

		if (bIsAddingLight)
		{
			Begin("Add Light", &bIsAddingLight);

			if (BeginCombo("Select Kind Of Light", lightKind[ullSelectedLightKind]))
			{
				for (size_t kindIdx = 0; kindIdx < IM_ARRAYSIZE(lightKind); ++kindIdx)
				{
					const bool is_selected = (ullSelectedLightKind == kindIdx);
					if (ImGui::Selectable(lightKind[kindIdx], is_selected))
						ullSelectedLightKind = kindIdx;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			switch (ullSelectedLightKind)
			{
			case LightKindSpot:
				ShowLightSpotSetting(
					fLightPos,
					fLightAngleDeg,
					fLightColor,
					&fFallOffStart,
					&fFallOffEnd,
					&fLightPower,
					&fSpotPower
				);
				break;
			case LightKindPoint:
				ShowLightPointSetting(
					fLightPos,
					fLightColor,
					&fFallOffStart,
					&fFallOffEnd,
					&fLightPower
				);
				break;
			default:
				break;
			}

			if (Button("OK"))
			{
				AddLight();
				bIsAddingLight = false;
			}
			SameLine();
			if (Button("Cancel"))
			{
				bIsAddingLight = false;
			}
			End();
		}

		if (bIsShowingLightInfo && spSelectedLight != nullptr)
		{
			Begin("Light Info", &bIsShowingLightInfo);
			spSelectedLight->AcceptLightSetting(this);
			End();
		}
	}
}

void LightManipulator::ShowLightSpotSetting(
	float* fLightPosIn,
	float* fLightAngleDegIn,
	float* fLightColorIn,
	float* fFallOffStartIn,
	float* fFallOffEndIn,
	float* fLightPower,
	float* fSpotPower
)
{
	float fFallOffEndLimit = *fFallOffStartIn + NEAR_CLOSE;

	DragFloat3("Light Position", fLightPosIn, 0.5f, -100.f, 100.f);
	DragFloat3("Light Angle(Deg)", fLightAngleDegIn, 0.5f, -180.f, 180.f);
	DragFloat3("Light Color RGB", fLightColorIn, 0.001f, 0.f, 1.f);
	DragFloat("Fall-Off Start Constant", fFallOffStartIn, 0.f, ILight::gLightNearZ, 100.f);
	DragFloat("Fall-Off End Constant", fFallOffEndIn, 0.1f, fFallOffEndLimit, 100.f);
	DragFloat("Light Power", fLightPower, 0.01f, 0.f, 10.f);
	DragFloat("Spot Power", fSpotPower, 0.01f, 1.f, 10.f);

	if (*fFallOffEndIn < fFallOffEndLimit) *fFallOffEndIn = fFallOffEndLimit;
}

void LightManipulator::ShowLightPointSetting(
	float* fLightPosIn,
	float* fLightColorIn,
	float* fFallOffStartIn,
	float* fFallOffEndIn,
	float* fLightPower
)
{
	DragFloat3("Light Position", fLightPosIn, 0.5f, -100.f, 100.f);
	DragFloat3("Light Color RGB", fLightColorIn, 0.001f, 0.f, 1.f);
	DragFloat("Fall-Off Start Constant", fFallOffStartIn, 0.01f, 0.f, 100.f);
	DragFloat("Fall-Off End Constant", fFallOffEndIn, 0.01f, *fFallOffStartIn, 100.f);
	DragFloat("Light Power", fLightPower, 0.001f, 0.f, 10.f);
}

void LightManipulator::AddLight()
{
	shared_ptr<ILight> pAddedLight = nullptr;
	switch (ullSelectedLightKind)
	{
	case LightKindSpot:
		pAddedLight = make_shared<SpotLight>(
			fLightPos[0], fLightPos[1], fLightPos[2],
			fLightAngleDeg[0], fLightAngleDeg[1], fLightAngleDeg[2],
			fLightColor[0], fLightColor[1], fLightColor[2],
			fFallOffStart, fFallOffEnd, fLightPower, fSpotPower
		);
		break;
	case LightKindPoint:
		pAddedLight = make_shared<PointLight>(
			fLightPos[0], fLightPos[1], fLightPos[2],
			fLightColor[0], fLightColor[1], fLightColor[2],
			fFallOffStart, fFallOffEnd, fLightPower
		);
		break;
	default:
		break;
	}
	InitLightVariable();
	pLights.push_back(pAddedLight);
}

void LightManipulator::VisitLightList(SpotLight& spotLight)
{
	SetLightList("Spot Light" + to_string(spotLight.GetLightID()));
}

void LightManipulator::VisitLightList(PointLight& pointLight)
{
	SetLightList("Point Light" + to_string(pointLight.GetLightID()));
}

void LightManipulator::SetLightList(const std::string& strLightName)
{
	const bool is_selected = (iLightSelectedIdx == (int)ullLightListCheckIdx);
	if (Selectable(strLightName.c_str(), is_selected))
	{
		iLightSelectedIdx = (int)ullLightListCheckIdx;
		spSelectedLight = pLights[ullLightListCheckIdx];
	}

	if (is_selected)
		SetItemDefaultFocus();
}

void LightManipulator::VisitLightSetting(SpotLight& spotLight)
{
	ShowLightSpotSetting(
		spotLight.GetPosition().m128_f32,
		spotLight.GetAngles(),
		spotLight.GetLightColor(),
		spotLight.GetLightFallOffStart(),
		spotLight.GetLightFallOffEnd(),
		spotLight.GetLightPower(),
		spotLight.GetSpotPower()
	);
}

void LightManipulator::VisitLightSetting(PointLight& pointLight)
{
	ShowLightPointSetting(
		pointLight.GetPosition().m128_f32,
		pointLight.GetLightColor(),
		pointLight.GetLightFallOffStart(),
		pointLight.GetLightFallOffEnd(),
		pointLight.GetLightPower()
	);
}
