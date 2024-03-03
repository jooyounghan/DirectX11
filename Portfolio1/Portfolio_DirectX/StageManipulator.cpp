#include "StageManipulator.h"
#include "ModelManipulator.h"

#include "ACamera.h"
#include "ILight.h"

using namespace ImGui;

StageManipulator::StageManipulator(
	UINT& uiWidthIn,
	UINT& uiHeightIn,
	ModelManipulator* pModelManipulatorIn
)
	: upCameraManipulator(std::make_unique<CameraManipulator>(uiWidthIn, uiHeightIn, pModelManipulatorIn)),
	upLightManipulator(std::make_unique<LightManipulator>())
{
}

StageManipulator::~StageManipulator()
{
}

void StageManipulator::PopAsDialog()
{
    Begin("Light And Camera Setting");
	upCameraManipulator->SetCameraGui();
	upLightManipulator->SetLightGui();
    End();
}

bool StageManipulator::IsGuiAvailable()
{
	return true;
}
