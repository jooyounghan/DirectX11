#pragma once
#include <memory>
#include <vector>
#include <unordered_map>

#include "BaseApp.h"

enum KeyCode
{
	Zero = 0x30,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	A = 0x41,
	B, C, D, E, F,
	G, H, I, J, K,
	L, M, N, O, P, 
	Q, R, S, T, U,
	V, W, X, Y, Z,
	NUM_KEY
};

typedef bool IsChecked;

class CameraInterface;
class ModelInterface;
class ObjectModel;
class CubeMapModel;

class ManageGuiInterface;

class LightManager;

class BaseModelDrawer;
class ModelOutlineDrawer;
class NormalVectorDrawer;
class CubeMapDrawer;

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
	
private:
	bool bIsNormalVectorDraw = false;
	bool bIsWireFrameDraw = false;

public:
	void ResizeSwapChain(const UINT& uiWidthIn, const UINT& uiHeightIn);

public:
	void CheckMouseHoveredModel();

private:
	std::shared_ptr<CameraInterface>				spMainCameras;
	std::vector<std::shared_ptr<CameraInterface>>	vSpCameras;

private:
	std::shared_ptr<ObjectModel>					spSelectedModel;
	std::shared_ptr<ObjectModel>					spTempSelectedModel;
	std::vector<std::shared_ptr<ObjectModel>>		vSpModels;

private:
	std::shared_ptr<LightManager>					spLightManager;

private:
	std::shared_ptr<CubeMapModel>					spCubeMap;

private:
	std::unique_ptr<BaseModelDrawer>				upModelDrawer;
	std::unique_ptr<ModelOutlineDrawer>				upModelOutlineDrawer;
	std::unique_ptr<NormalVectorDrawer>				upNormalVectorDrawer;
	std::unique_ptr<CubeMapDrawer>					upCubeMapDrawer;

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

