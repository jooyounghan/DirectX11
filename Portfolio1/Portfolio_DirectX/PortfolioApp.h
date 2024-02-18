#pragma once

#include "BaseApp.h"

#include "ModelManipulator.h"
#include "FileManipulator.h"
#include "StageManipulator.h"

#include "ModelRenderer.h"
#include "LightRenderer.h"
#include "NormalVectorRenderer.h"

#include <unordered_map>

class PortfolioApp : public BaseApp
{
public:
	PortfolioApp(const UINT& uiWidthIn, const UINT& uiHeightIn);
	virtual ~PortfolioApp();

public:
	virtual void Init() override;
	virtual void Update(const float& fDelta) override;
	virtual void Render() override;
	virtual void Run() override;
	virtual void Quit() override;

public:
	void InitImGUI();
	void SetImGUIRendering();
	void RenderImGUI();
	void QuitImGUI();

public:
	virtual LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

public:
	std::unique_ptr<ModelManipulator> upModelManipulator;
	std::unique_ptr<FileManipulator> upFileManipulator;
	std::unique_ptr<StageManipulator> upStageManipulator;

public:
	ModelRenderer modelRenderer;
	LightRenderer lightRenderer;
	NormalVectorRenderer normalVectorRenderer;

private:
	class AIBLModel* pIBLModel;
	std::unordered_map<uint32_t, AStaticMesh*> pModels;

public:
	void AddModel(AStaticMesh* pModel);
};

