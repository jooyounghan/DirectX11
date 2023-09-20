#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include <memory>
#include <vector>
#include <unordered_map>

typedef bool IsChecked;

#include "BaseApp.h"


class ICamera;
class ILight;
class ModelInterface;
struct LightSet;

class PortfolioApp : public BaseApp
{
public:
	PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn);
	~PortfolioApp();

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Run() override;
	virtual void Quit() override;

public:
	void InitImGUI();
	void SetImGUIRendering();
	void UpdateGUI();
	void RenderImGUI();
	void QuitImGUI();

public:
	void SetModelManageWnd();
	void SetLightManageWnd();

public:
	void SetLightAddMenu();
	void SetLightSelectorMenu();
	void SetLightSettingMenu();

public:
	void SetDirectionalLightMenu(LightSet* pLightSet);
	void SetPointLightMenu(LightSet* pLightSet);
	void SetSpotLightMenu(LightSet* pLightSet);

public:
	void ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn);

public:
	void CheckMouseHoveredModel();

private:	
	std::shared_ptr<ICamera>				pMainCamera;
	std::shared_ptr<ModelInterface>			pSelectedModel;
	std::shared_ptr<ModelInterface>			pTempSelectedModel;
	std::shared_ptr<ILight>					pSelectedLight;

private:
	std::vector<std::shared_ptr<ICamera>>					vCameras;
	std::unordered_map<std::shared_ptr<ILight>, IsChecked>	umLights;
	std::unique_ptr<class ModelDrawer>						modelDrawer;
	std::unique_ptr<class ModelOutlineDrawer>				modelOutlineDrawer;
	std::vector<std::shared_ptr<ModelInterface>>			vModels;

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

