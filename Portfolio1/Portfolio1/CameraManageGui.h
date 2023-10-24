#pragma once

#include <memory>

#include "ManageGuiInterface.h"

class CameraInterface;

class CameraManageGui : public ManageGuiInterface
{
public:
	CameraManageGui(std::shared_ptr<CameraInterface>& spSelectedCameraIn);
	~CameraManageGui();

private:
	std::shared_ptr<CameraInterface>& spSelectedCamera;

public:
	virtual void RenderGui() override;

private:
	struct {
		float fTempBlendStrength;
		float fTempExposure;
		float fTempGamma;
	} sProperties;

private:
	void SetCameraPostProcess();
};

