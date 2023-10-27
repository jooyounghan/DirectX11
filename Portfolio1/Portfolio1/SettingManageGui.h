#pragma once
#include "ManageGuiInterface.h"

class SettingManageGui : public ManageGuiInterface
{
public:
	SettingManageGui();
	virtual ~SettingManageGui() override;

public:
	virtual void RenderGui() override;

private:
	void SetDrawNormalVector();

private:
	bool bIsNormalVectorDraw;

public:
	inline const bool& IsNormalVectorDraw() { return bIsNormalVectorDraw; }
};

