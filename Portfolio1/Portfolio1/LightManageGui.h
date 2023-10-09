#pragma once

#include <memory>
#include "ManageGuiInterface.h"

class LightManager;

class LightManageGui : public ManageGuiInterface
{
public:
	LightManageGui(
		std::shared_ptr<LightManager>&
		spLightManagerIn);
	~LightManageGui();

private:
	std::shared_ptr<LightManager>& spLightManager;

public:
	virtual void RenderGui() override;

private:
	void SetLightAddMenu();
	void SetLightSelectorMenu();
	void SetLightSettingMenu();

private:
	void SetDirectionalLightMenu(struct LightSet* pLightSet);
	void SetPointLightMenu(struct LightSet* pLightSet);
	void SetSpotLightMenu(struct LightSet* pLightSet);


private:
	bool		bLightTypeCheckFlag[3];
	const char* pItems[3];
	int			iSelectedIdx;

};

