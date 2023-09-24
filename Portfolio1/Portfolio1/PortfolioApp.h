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

class LightManager;
class FileManager;

class BaseModelDrawer;
class ModelOutlineDrawer;

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
	std::unique_ptr<ModelManageGui>		upModelManageGui;
	std::unique_ptr<LightManageGui>		upLightManageGui;
	std::unique_ptr<FileManageGui>		upFileManageGui;

public:
	void ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn);

public:
	void CheckMouseHoveredModel();

private:
	std::unique_ptr<FileManager>		upFileManager;

private:	
	std::shared_ptr<CameraInterface>		spMainCamera;
	std::shared_ptr<ModelInterface>			spSelectedModel;
	std::shared_ptr<ModelInterface>			spTempSelectedModel;

private:
	std::vector<std::shared_ptr<CameraInterface>>	vSpCameras;
	std::vector<std::shared_ptr<ModelInterface>>	vSpModels;
	std::shared_ptr<LightManager>					spLightManager;

private:
	std::unique_ptr<BaseModelDrawer>				upModelDrawer;
	std::unique_ptr<ModelOutlineDrawer>				upModelOutlineDrawer;


public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

