#pragma once
#include "ManageGuiInterface.h"

class SettingManageGui : public ManageGuiInterface
{
public:
	SettingManageGui(bool& bIsNormalVectorDrawIn, bool& bIsWireFrameDrawIn);
	virtual ~SettingManageGui() override;

public:
	virtual void RenderGui() override;

private:
	void SetDrawNormalVector();
	void SetDrawWireFrame();

private:
	bool& bIsNormalVectorDraw;
	bool& bIsWireFrameDraw;

public:
	inline const bool& IsNormalVectorDraw() { return bIsNormalVectorDraw; }
};

