#include "CameraManipulator.h"

#include "DefineVar.h"
#include "PickableCamera.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <DirectXMath.h>

using namespace std;
using namespace ImGui;

const char* CameraManipulator::cameraKind[] = { "Filtered Camera(No Model Picking)", "Filtered Camera(Model Picking)" };

CameraManipulator::CameraManipulator(
	UINT& uiWidthIn, 
	UINT& uiHeightIn
)
	:
	uiWidth(uiWidthIn),
	uiHeight(uiHeightIn),
	spSelectedCamera(nullptr),
	bIsAddingCamera(false),
	bIsShowingCameraInfo(false),
	iCameraSelectedIdx(NOT_SELECTED),
	iFilterSelectedIdx(NOT_SELECTED),
	ullCameraListCheckIdx(0),
	ullSelectedModelID(0)
{
	InitCameraVariable();
	AutoZeroMemory(fCameraPos);
	AutoZeroMemory(fCameraAngle);
}

CameraManipulator::~CameraManipulator()
{
}

void CameraManipulator::InitCameraVariable()
{
	uiCameraWidth = uiWidth;
	uiCameraHeight = uiHeight;
	fFovAngleDegree = 70.f;
	fNearPlane = 0.001f;
	fFarPlane = 1000.f;
	bSupportMultiSampled = false;
	bSupportHDR = false;
}

void CameraManipulator::SetCameraGui()
{
	if (CollapsingHeader("Camera Setting", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		BeginGroup();
		const ImVec2 buttonSize = ImVec2(GetWindowContentRegionWidth() / 2.f, GetTextLineHeight() * 1.1f);
		if (Button("Add Camera", buttonSize))
		{
			bIsAddingCamera = true;
		}
		if (Button("Remove Camera", buttonSize) && iCameraSelectedIdx >= 0)
		{
			spSelectedCamera = nullptr;
			pCameras.erase(pCameras.begin() + iCameraSelectedIdx);
			iCameraSelectedIdx = NOT_SELECTED;
		}
		if (Button("Camera Info", buttonSize))
		{
			bIsShowingCameraInfo = true;
		}
		EndGroup();

		SameLine();

		if (BeginListBox("Camera List", ImVec2(GetWindowContentRegionWidth() / 2.f, 3.f * GetTextLineHeightWithSpacing())))
		{
			for (ullCameraListCheckIdx = 0; ullCameraListCheckIdx < pCameras.size(); ++ullCameraListCheckIdx)
			{
				pCameras[ullCameraListCheckIdx]->AcceptCameraList(this);
			}
			EndListBox();
		}

		if (bIsAddingCamera)
		{
			Begin("Add Camera", &bIsAddingCamera);

			if (BeginCombo("Select Kind Of Light", cameraKind[ullSelectedCameraKind]))
			{
				for (size_t kindIdx = 0; kindIdx < IM_ARRAYSIZE(cameraKind); ++kindIdx)
				{
					const bool is_selected = (ullSelectedCameraKind == kindIdx);
					if (ImGui::Selectable(cameraKind[kindIdx], is_selected))
						ullSelectedCameraKind = kindIdx;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ShowCameraSetting(
				fCameraPos,
				fCameraAngle,
				&uiCameraWidth,
				&uiCameraHeight,
				&fFovAngleDegree,
				&fNearPlane,
				&fFarPlane,
				&bSupportMultiSampled,
				&bSupportHDR
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
			End();
		}


		if (bIsShowingCameraInfo && spSelectedCamera != nullptr)
		{
			spSelectedCamera->AcceptCameraInformation(this);
		}
	}
}

void CameraManipulator::AddCamera()
{
	shared_ptr<ACamera> pAddedCamera = nullptr;
	switch (ullSelectedCameraKind)
	{
	case CameraKindFiltered:
		pAddedCamera = make_shared<FilteredCamera>(
			fCameraPos[0], fCameraPos[1], fCameraPos[2],
			fCameraAngle[0], fCameraAngle[1], fCameraAngle[2],
			(UINT)uiCameraWidth, (UINT)uiCameraHeight,
			fFovAngleDegree,
			fNearPlane, fFarPlane, (UINT)bSupportMultiSampled,
			bSupportHDR ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D24_UNORM_S8_UINT
		);
		break;
	case CameraKindPickable:
		pAddedCamera = make_shared<PickableCamera>(
			fCameraPos[0], fCameraPos[1], fCameraPos[2],
			fCameraAngle[0], fCameraAngle[1], fCameraAngle[2],
			(UINT)uiCameraWidth, (UINT)uiCameraHeight,
			fFovAngleDegree,
			fNearPlane, fFarPlane, (UINT)bSupportMultiSampled,
			bSupportHDR ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D24_UNORM_S8_UINT
		);
		break;
	default:
		break;
	}

	InitCameraVariable();
	pCameras.push_back(pAddedCamera);
}

void CameraManipulator::ResizeSelectedCamera(const UINT uiWidthIn, const UINT uiHeightIn)
{
	if (spSelectedCamera != nullptr)
	{
		spSelectedCamera->Resize(uiWidthIn, uiHeightIn);
	}
}

void CameraManipulator::ProcWindowMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (spSelectedCamera != nullptr)
	{
		switch (msg) {
		case WM_MOUSEMOVE:
			spSelectedCamera->ManageMouseInput(((int)(short)LOWORD(lParam)), ((int)(short)HIWORD(lParam)));
			break;
		case WM_KEYUP:
			switch (wParam)
			{
			case EKeyCode::W:
			case EKeyCode::A:
			case EKeyCode::S:
			case EKeyCode::D:
				spSelectedCamera->Release((EKeyCode)wParam);
				break;
			}
			break;
		case WM_KEYDOWN:
			switch (wParam) {
			case EKeyCode::W:
			case EKeyCode::A:
			case EKeyCode::S:
			case EKeyCode::D:
				spSelectedCamera->Press((EKeyCode)wParam);
				break;
			case EKeyCode::F:
				spSelectedCamera->Toggle((EKeyCode)wParam);
				break;
			}
			break;
		case WM_LBUTTONDOWN:
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			{
				spSelectedCamera->AcceptLButtonDown(this, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
			}
			break;
		case WM_LBUTTONUP:
			break;
		}
	}
}

void CameraManipulator::ShowCameraSetting(
	float* pCameraPosIn, 
	float* pCameraAngleIn, 
	int* pViewportWidth, 
	int* pViewportHeight, 
	float* pFovAngle, 
	float* pNearZ, float* pFarZ, 
	bool* pIsSupportMultiSampled, 
	bool* pSupportHDR
)
{
	DragFloat3("Camera Position", pCameraPosIn, 0.5f, -100.f, 100.f);
	DragFloat3("Camera Angles(Deg)", pCameraAngleIn, 0.5f, -180.f, 180.f);
	DragInt("ViewPort Width", pViewportWidth, 1, 100, uiWidth);
	DragInt("ViewPort Height", pViewportHeight, 1, 100, uiHeight);
	DragFloat("Fov Angle", pFovAngle, 0.05f, 30.f, 150.f);
	DragFloat("Near Z Plane", pNearZ, 0.0001f, 0.001f, 1.f, "%.4f");
	DragFloat("Far Z Plane", pFarZ, 1.f, 10.f, 1000.f, "%.f");

	Checkbox("Support MultiSampling", pIsSupportMultiSampled);
	SameLine();
	Checkbox("Support HDR", pSupportHDR);
}

void CameraManipulator::VisitCameraList(FilteredCamera& pFiltered)
{
	SetCameraList("Filterted Camera" + to_string(pFiltered.GetCameraID()));
}

void CameraManipulator::VisitCameraList(PickableCamera& pPickable)
{
	SetCameraList("Pickable Camera" + to_string(pPickable.GetCameraID()));
}

void CameraManipulator::SetCameraList(const string& strCameraName)
{
	const bool is_selected = (iCameraSelectedIdx == (int)ullCameraListCheckIdx);
	if (Selectable(strCameraName.c_str(), is_selected))
	{
		iCameraSelectedIdx = (int)ullCameraListCheckIdx;
		if (spSelectedCamera != nullptr)
		{
			spSelectedCamera->LinkWithBackBuffer(false);
		}
		spSelectedCamera = pCameras[ullCameraListCheckIdx];
		spSelectedCamera->LinkWithBackBuffer(true);
	}

	if (is_selected)
		SetItemDefaultFocus();
}

void CameraManipulator::VisitCameraInfo(FilteredCamera& filtered)
{
	Begin("Camera Information", &bIsShowingCameraInfo);

	int iViewportWidth = (int)filtered.sViewPort.Width;
	int iViewportHeight = (int)filtered.sViewPort.Height;
	bool bIsSelectedSupportMultiSampled = filtered.ViewableRenderTarget::uiNumQualityLevels;
	bool bIsSelectedSupportHDR = filtered.ViewableRenderTarget::GetFormat() == DXGI_FORMAT_R16G16B16A16_FLOAT;

	BeginDisabled();
	ShowCameraSetting(
		filtered.GetPosition().m128_f32,
		(float*)&filtered.sAnglesDegree,
		&iViewportWidth,
		&iViewportHeight,
		&filtered.fFovDegree,
		&filtered.fNearZ,
		&filtered.fFarZ,
		&bIsSelectedSupportMultiSampled,
		&bIsSelectedSupportHDR
	);
	EndDisabled();

	const vector<unique_ptr<class AFilter>>& filters = filtered.GetFilters();

	BeginGroup();
	if (Button("Add Blur Filter"))
	{
		filtered.AddBlurState();
	}
	EndGroup();

	SameLine();

	if (BeginListBox("Filter Lists", ImVec2(GetWindowContentRegionWidth() / 2.f, 3 * GetTextLineHeightWithSpacing())))
	{
		filtered.AcceptFilterList(this);
		for (ullFilterListCheckIdx = 0; ullFilterListCheckIdx < filters.size(); ++ullFilterListCheckIdx)
		{
			filters[ullFilterListCheckIdx]->AcceptFilterList(this);
		}
		EndListBox();
	}
	End();
}

void CameraManipulator::VisitCameraInfo(PickableCamera& pickable)
{
	FilteredCamera& asFiltered = pickable;
	VisitCameraInfo(asFiltered);
}

void CameraManipulator::VisitFilterList(BlurFilter& blurFilter)
{
	SetFilterList("Blur Filter");
}

void CameraManipulator::VisitFilterList(ACamera& camera)
{
	SetFilterList("Camera Resolve Filter");
}

void CameraManipulator::VisitFilterList(PickableCamera& pickable)
{
	SetFilterList("ID Picking Resolve Filter");
}

void CameraManipulator::SetFilterList(const string& strFilterName)
{
	const bool is_selected = (iFilterSelectedIdx == (int)ullFilterListCheckIdx);
	if (Selectable(strFilterName.c_str(), is_selected))
	{
		iFilterSelectedIdx = (int)ullFilterListCheckIdx;
	}

	if (is_selected)
		SetItemDefaultFocus();
}

void CameraManipulator::VisitLButtonDown(
	class FilteredCamera& fFiltered,
	const int& xPosIn,
	const int& yPosIn
)
{

}
void CameraManipulator::VisitLButtonDown(
	class PickableCamera& pickable,
	const int& xPosIn,
	const int& yPosIn
)
{
	pickable.SetMousePos(xPosIn, yPosIn);
	ullSelectedModelID = pickable.GetPickedID();
}