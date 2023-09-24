#pragma once

#include <memory>

class LightManager;

class LightManageGui
{
public:
	LightManageGui(
		std::shared_ptr<LightManager>&
		spLightManagerIn);
	~LightManageGui();

private:
	std::shared_ptr<LightManager>& spLightManager;

public:
	void RenderGui();

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

