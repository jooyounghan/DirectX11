#pragma once
#include "IGuiMenu.h"
#include "LightManipulator.h"
#include "CameraManipulator.h"

class StageManipulator : public IGuiMenu
{
public:
	StageManipulator(
		UINT& uiWidthIn,
		UINT& uiHeightIn,
		class ModelManipulator* pModelManipulatorIn
	);
	virtual ~StageManipulator();

protected:
	std::unique_ptr<CameraManipulator> upCameraManipulator;
	std::unique_ptr<LightManipulator> upLightManipulator;


public:
	inline CameraManipulator* const GetCameraManipulator() { return upCameraManipulator.get(); }
	inline LightManipulator* const GetLightManipulator() { return upLightManipulator.get(); }

public:
	virtual void PopAsDialog() override;
	virtual bool IsGuiAvailable() override;
};

