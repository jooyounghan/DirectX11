#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "CameraManageGui.h"

#include "CameraInterface.h"
#include "PostProcess.h"
#include "DefineVar.h"

using namespace ImGui;

CameraManageGui::CameraManageGui(std::shared_ptr<CameraInterface>& spSelectedCameraIn)
	: spSelectedCamera(spSelectedCameraIn)
{
	AutoZeroMemory(sProperties);
	sProperties.fTempExposure = 1.f;
	sProperties.fTempGamma = 2.2f;
}

CameraManageGui::~CameraManageGui()
{
}

void CameraManageGui::RenderGui()
{
	Begin("Camera Mananger");

	SetCameraPostProcess();

	End();
}

void CameraManageGui::SetCameraPostProcess()
{
	ImGui::SliderFloat("Blend Strength", &sProperties.fTempBlendStrength, 0.f, 1.f);
	ImGui::SliderFloat("Exposure", &sProperties.fTempExposure, 0.f, 5.f);
	ImGui::SliderFloat("Gamma", &sProperties.fTempGamma, 0.f, 5.f);
	spSelectedCamera->pPostProcess->SetBlendProperties(
		sProperties.fTempBlendStrength,
		sProperties.fTempExposure,
		sProperties.fTempGamma
	);
}
