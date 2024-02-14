#include "StageManipulator.h"

#include "DefineVar.h"
#include "directxmath/DirectXMath.h"

#include "PickableCamera.h"

#include "SpotLight.h"
#include "PointLight.h"

using namespace ImGui;

StageManipulator::StageManipulator(
	UINT& uiWidthIn,
	UINT& uiHeightIn,
	std::vector<std::shared_ptr<ILight>>& pLightsIn,
	std::vector<std::shared_ptr<ACamera>>& pCamerasIn
) : 
	uiWidth(uiWidthIn),
	uiHeight(uiHeightIn),
	pLights(pLightsIn), 
	pCameras(pCamerasIn), 
	spSelectedLight(nullptr),
	spSelectedCamera(nullptr),

	uiCameraWidth(uiWidth),
	uiCameraHeight(uiHeight),
	fFovAngle(70.f),
	fNearPlane(0.0001f),
	fFarPlane(1000.f),
	bSupportMultiSampled(false),
	bSupportHDR(false)
{
	AutoZeroMemory(fCameraPos);
	AutoZeroMemory(fCameraAngle);
	AutoZeroMemory(fLightPos);
}

StageManipulator::~StageManipulator()
{
}

void StageManipulator::PopAsDialog()
{
    Begin("Light And Camera Setting");
	SetCameraGui();
	SetLightGui();
    End();
}

bool StageManipulator::IsGuiAvailable()
{
	return true;
}

void StageManipulator::SetCameraGui()
{
	if (CollapsingHeader("Camera Setting", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::TreeNode("Add Camera"))
		{
			DragFloat3("Camera Position", fCameraPos, 0.5f, -100.f, 100.f);
			DragFloat3("Camera Angles(Deg)", fCameraAngle, 0.5f, -180.f, 180.f);
			DragInt("Camera ViewPort Width", &uiCameraWidth, 1, 100, uiWidth);
			DragInt("Camera ViewPort Height", &uiCameraHeight, 1, 100, uiHeight);
			DragFloat("Fov Angle", &fFovAngle, 0.05f, 30.f, 150.f);
			DragFloat("Near Z Plane", &fNearPlane, 0.0001f, 0.0001f, 1.f, "%.4f");
			DragFloat("Far Z Plane", &fFarPlane, 1.f, 10.f, 1000.f, "%.f");

			Checkbox("Support MultiSampling", &bSupportMultiSampled);
			SameLine();
			Checkbox("Support HDR", &bSupportHDR);
			SameLine();
			Checkbox("Support Picking", &bSupportPicking);

			if (Button("Clicked"))
			{
				std::shared_ptr<ACamera> pAddedCamera = nullptr;
				if (bSupportPicking)
				{
					pAddedCamera = std::make_unique<PickableCamera>(
						fCameraPos[0], fCameraPos[1], fCameraPos[2],
						fCameraAngle[0], fCameraAngle[1], fCameraAngle[2],
						(UINT)uiCameraWidth, (UINT)uiCameraHeight,
						DirectX::XMConvertToRadians(fFovAngle),
						fNearPlane, fFarPlane, (UINT)bSupportMultiSampled,
						bSupportHDR ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_R8G8B8A8_UNORM,
						DXGI_FORMAT_D24_UNORM_S8_UINT
					);
				}
				else
				{
					pAddedCamera = std::make_unique<FilteredCamera>(
						fCameraPos[0], fCameraPos[1], fCameraPos[2],
						fCameraAngle[0], fCameraAngle[1], fCameraAngle[2],
						(UINT)uiCameraWidth, (UINT)uiCameraHeight,
						DirectX::XMConvertToRadians(fFovAngle),
						fNearPlane, fFarPlane, (UINT)bSupportMultiSampled,
						bSupportHDR ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_R8G8B8A8_UNORM,
						DXGI_FORMAT_D24_UNORM_S8_UINT
					);
				}
				pCameras.push_back(pAddedCamera);
			}

			ImGui::TreePop();
			ImGui::Spacing();
		}
		if (ImGui::TreeNode("Manage Camera"))
		{
			if (ImGui::BeginListBox("Camera List"))
			{
				for (int idx = 0; idx < (int)pCameras.size(); ++idx)
				{
					const bool is_selected = (iCameraSelectedIdx == idx);
					if (ImGui::Selectable(pCameras[idx]->GetCameraName().c_str(), is_selected))
					{
						iCameraSelectedIdx = idx;
						if (spSelectedCamera != nullptr)
						{
							spSelectedCamera->LinkWithBackBuffer(false);
						}
						spSelectedCamera = pCameras[idx];
						spSelectedCamera->LinkWithBackBuffer(true);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			ImGui::TreePop();
			ImGui::Spacing();
		}

		Separator();
	}
}

void StageManipulator::SetLightGui()
{
	if (CollapsingHeader("Light Setting", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		Separator();
		DragFloat3("Light Position", fLightPos, 0.5f, -100.f, 100.f);
	}
}
