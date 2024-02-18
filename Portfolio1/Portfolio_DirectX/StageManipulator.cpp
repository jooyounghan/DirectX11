#include "StageManipulator.h"


#include "ACamera.h"
#include "ILight.h"

using namespace ImGui;

StageManipulator::StageManipulator(
	UINT& uiWidthIn,
	UINT& uiHeightIn
)
	: upCameraManipulator(std::make_unique<CameraManipulator>(uiWidthIn, uiHeightIn)),
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
