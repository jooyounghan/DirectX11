#pragma once

#include <vector>
#include <memory>
#include "ManageGuiInterface.h"

struct PointLightSet;
struct SpotLightSet;

class LightManager;

enum ELightType : unsigned int
{
	PointLightType,
	SpotLightType
};

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
	void SetLightSelectorTab();
	void SetLightSettingMenu();

private:
	void SetPointLightMenu(PointLightSet* pPointLightSet);
	void SetSpotLightMenu(SpotLightSet* pSpotLightSet);

private:
	struct PointLightSet* pTempPointLightSet;
	struct SpotLightSet* pTempSpotLightSet;

private:
	bool				bLightTypeCheckFlag[2];
	const char*			pItems[2];
	
private:
	unsigned int	uiSelLightType;
	unsigned int	uiSelTabIdx;

private:
	template<typename Ptr>
	void SetLightSelector(const char* label, const std::vector<Ptr*>& vLightsIn, const size_t& idxSel, size_t& idxSelected);
};