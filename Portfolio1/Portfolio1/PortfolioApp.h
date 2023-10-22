#pragma once
#include <memory>
#include <vector>
#include <unordered_map>

#include "BaseApp.h"

typedef bool IsChecked;

class CameraInterface;
class ModelInterface;

class ModelManageGui;
class LightManageGui;
class FileManageGui;
class SettingManageGui;

class LightManager;
class FileManager;

class BaseModelDrawer;
class ModelOutlineDrawer;
class NormalVectorDrawer;

class PostProcess;

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
	void DoPostProcess();

public:
	void InitImGUI();
	void SetImGUIRendering();
	void RenderImGUI();
	void QuitImGUI();

private:
	std::unique_ptr<ModelManageGui>		upModelManageGui;
	std::unique_ptr<LightManageGui>		upLightManageGui;
	std::unique_ptr<FileManageGui>		upFileManageGui;
	std::unique_ptr<SettingManageGui>	upSettingManageGui;

public:
	void ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn);

public:
	void CheckMouseHoveredModel();

private:
	std::unique_ptr<FileManager>					upFileManager;

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

private:
	std::unique_ptr<PostProcess>					upPostProcess;

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

