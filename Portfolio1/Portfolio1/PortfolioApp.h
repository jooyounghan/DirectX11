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


class CameraInterface;
class ModelInterface;

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
	void SetDirectionalLightMenu(struct LightSet* pLightSet);
	void SetPointLightMenu(struct LightSet* pLightSet);
	void SetSpotLightMenu(struct LightSet* pLightSet);

public:
	void ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn);

public:
	void CheckMouseHoveredModel();

private:
	std::unique_ptr<class FileManager>		upFileManager;

private:	
	std::shared_ptr<CameraInterface>		spMainCamera;
	std::shared_ptr<ModelInterface>			spSelectedModel;
	std::shared_ptr<ModelInterface>			spTempSelectedModel;

private:
	std::vector<std::shared_ptr<CameraInterface>>					spvCameras;
	std::vector<std::shared_ptr<ModelInterface>>					spvModels;
	std::unique_ptr<class LightManager>								upLightManager;

private:
	std::unique_ptr<class BaseModelDrawer>							upModelDrawer;
	std::unique_ptr<class ModelOutlineDrawer>						upModelOutlineDrawer;


public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

