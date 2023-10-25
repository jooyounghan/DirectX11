#pragma once
#include <memory>
#include <vector>
#include <unordered_map>

#include "BaseApp.h"

typedef bool IsChecked;

class CameraInterface;
class ModelInterface;
class ManageGuiInterface;

class LightManager;

class BaseModelDrawer;
class ModelOutlineDrawer;
class NormalVectorDrawer;

template<typename T>
class Canvas;

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
	void RenderImGUI();
	void QuitImGUI();

private:
	std::vector<std::unique_ptr<ManageGuiInterface>> vUpManageGuis;

public:
	void ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn);

public:
	void CheckMouseHoveredModel();

private:
	std::shared_ptr<CameraInterface>				spMainCameras;
	std::vector<std::shared_ptr<CameraInterface>>	vSpCameras;

private:
	std::shared_ptr<ModelInterface>					spSelectedModel;
	std::shared_ptr<ModelInterface>					spTempSelectedModel;
	std::vector<std::shared_ptr<ModelInterface>>	vSpModels;

private:
	std::shared_ptr<LightManager>					spLightManager;

private:
	std::unique_ptr<Canvas<BaseModelDrawer>>		upBaseCanvas;
	std::unique_ptr<Canvas<ModelOutlineDrawer>>		upModelOutlineCanvas;
	std::unique_ptr<Canvas<NormalVectorDrawer>>		upNVCanvas;

private:
	std::unique_ptr<BaseModelDrawer>				upModelDrawer;
	std::unique_ptr<ModelOutlineDrawer>				upModelOutlineDrawer;
	std::unique_ptr<NormalVectorDrawer>				upNormalVectorDrawer;

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

