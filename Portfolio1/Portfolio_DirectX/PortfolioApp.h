#pragma once

#include "BaseApp.h"

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
};

