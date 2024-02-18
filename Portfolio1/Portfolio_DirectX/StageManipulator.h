#pragma once
#include "IGuiMenu.h"
#include "LightManipulator.h"
#include "CameraManipulator.h"

class StageManipulator : public IGuiMenu
{
public:
	StageManipulator(
		UINT& uiWidthIn,
		UINT& uiHeightIn
	);
	virtual ~StageManipulator();

protected:
	std::unique_ptr<CameraManipulator> upCameraManipulator;
	std::unique_ptr<LightManipulator> upLightManipulator;


public:
	inline const std::vector<std::shared_ptr<class ACamera>>& GetCameras() { return upCameraManipulator->GetCameras(); }
	inline const std::vector<std::shared_ptr<class ILight>>& GetLights() { return upLightManipulator->GetLights(); }
	inline ACamera* GetSelectedCamera() { return upCameraManipulator->GetSelectedCamera(); }
	inline ILight* GetSelectedLight() { return upLightManipulator->GetSelectedLight(); }

public:
	virtual void PopAsDialog() override;
	virtual bool IsGuiAvailable() override;
};

