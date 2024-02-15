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
	bIsAddingCamera(false),
	bIsShowingCameraInfo(false),
	iCameraSelectedIdx(NOT_SELECTED)
{
	InitCameraVariable();

	AutoZeroMemory(fCameraPos);
	AutoZeroMemory(fCameraAngle);
	AutoZeroMemory(fLightPos);
}

StageManipulator::~StageManipulator()
{
}

void StageManipulator::InitCameraVariable()
{
	uiCameraWidth = uiWidth;
	uiCameraHeight = uiHeight;
	fFovAngle = 70.f;
	fNearPlane = 0.0001f;
	fFarPlane = 1000.f;
	bSupportMultiSampled = false;
	bSupportHDR = false;
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
		if (bIsAddingCamera)
		{
			ImGui::Begin("Add Camera", &bIsAddingCamera);
			
			ShowCameraSetting(
				true, fCameraPos, fCameraAngle, 
				&uiCameraWidth, &uiCameraHeight, 
				&fFovAngle, &fNearPlane, &fFarPlane, 
				&bSupportMultiSampled, &bSupportHDR, &bSupportPicking
			);

			if (Button("OK"))
			{
				AddCamera();
				bIsAddingCamera = false;
			}
			SameLine();
			if (Button("Cancel"))
			{
				bIsAddingCamera = false;
			}
			ImGui::End();
		}

		if (Button("Add Camera"))
		{
			bIsAddingCamera = true;
		}
		SameLine();
		if (Button("Remove Camera") && iCameraSelectedIdx >= 0)
		{
			spSelectedCamera = nullptr;
			pCameras.erase(pCameras.begin() + iCameraSelectedIdx);
			iCameraSelectedIdx = NOT_SELECTED;
		}
		SameLine();

		if (Button("Get Camera Info"))
		{
			bIsShowingCameraInfo = true;
		}

		if (bIsShowingCameraInfo)
		{
			ImGui::Begin("Camera Information", &bIsShowingCameraInfo);
			if (spSelectedCamera != nullptr)
			{
				UpdateSelectedVariable();
				ShowCameraSetting(
					false, spSelectedCamera->GetPosition().m128_f32, 
					(float*)&spSelectedCamera->sAnglesDegree,
					&iSelectedVPWidth, &iSelectedVPHeight,
					&fSelecedFovDegree,
					&spSelectedCamera->fNearZ, 
					&spSelectedCamera->fFarZ,
					&bIsSelectedSupportMultiSampled, 
					&bIsSelectedSupportHDR, 
					&bIsSelectedSupportPicking
				);
			}
			ImGui::End();
		}

		if (ImGui::BeginListBox("Camera List", ImVec2(-FLT_MIN, 3 * ImGui::GetTextLineHeightWithSpacing())))
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

		Separator();
	}
}

void StageManipulator::AddCamera()
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
	InitCameraVariable();
	pCameras.push_back(pAddedCamera);
}

void StageManipulator::ShowCameraSetting(
	const bool& bIsEditable, 
	float* pCameraPosIn, 
	float* pCameraAngleIn,
	int* pViewportWidth,
	int* pViewportHeight,
	float* pFovAngle,
	float* pNearZ,
	float* pFarZ,
	bool* pIsSupportMultiSampled,
	bool* pSupportHDR,
	bool* pSupportPicking
)
{
	ImGui::BeginDisabled(!bIsEditable);
	DragFloat3("Camera Position", pCameraPosIn, 0.5f, -100.f, 100.f);
	DragFloat3("Camera Angles(Deg)", pCameraAngleIn, 0.5f, -180.f, 180.f);
	DragInt("ViewPort Width", pViewportWidth, 1, 100, uiWidth);
	DragInt("ViewPort Height", pViewportHeight, 1, 100, uiHeight);
	DragFloat("Fov Angle", pFovAngle, 0.05f, 30.f, 150.f);
	DragFloat("Near Z Plane", pNearZ, 0.0001f, 0.0001f, 1.f, "%.4f");
	DragFloat("Far Z Plane", pFarZ, 1.f, 10.f, 1000.f, "%.f");

	Checkbox("Support MultiSampling", pIsSupportMultiSampled);
	SameLine();
	Checkbox("Support HDR", pSupportHDR);
	SameLine();
	Checkbox("Support Picking", pSupportPicking);
	ImGui::EndDisabled();
}

void StageManipulator::UpdateSelectedVariable()
{
	iSelectedVPWidth = static_cast<int>(spSelectedCamera->uiWidth);
	iSelectedVPHeight = static_cast<int>(spSelectedCamera->uiHeight);
	fSelecedFovDegree = DirectX::XMConvertToDegrees(spSelectedCamera->fFovRadian);
	bIsSelectedSupportMultiSampled = spSelectedCamera->ViewableRenderTarget::GetQualityLevels() == 1;
	bIsSelectedSupportHDR = spSelectedCamera->ViewableRenderTarget::GetFormat() == DXGI_FORMAT_R16G16B16A16_FLOAT;
	bIsSelectedSupportPicking = dynamic_cast<PickableCamera*>(spSelectedCamera.get()) != nullptr;
}

void StageManipulator::SetLightGui()
{
	if (CollapsingHeader("Light Setting", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		Separator();
		DragFloat3("Light Position", fLightPos, 0.5f, -100.f, 100.f);
	}
}
