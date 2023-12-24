#pragma once

#include <memory>
#include "ManageGuiInterface.h"

class LightManager;

class LightManageGui : public ManageGuiInterface
{
public:
	LightManageGui(LightManager* pLightManagerIn);
	virtual ~LightManageGui() override;

private:
	LightManager* pLightManager;

public:
	virtual void RenderGui() override;

private:
	void SetLightAddMenu();
	void SetLightSelectorMenu();
	void SetLightSettingMenu();

private:
	void SetPointLightMenu();
	void SetSpotLightMenu();


private:
	struct LightSet*	pTempLightSet;
	bool				bLightTypeCheckFlag[2];
	const char*			pItems[2];
	int					iSelectedIdx;

};

