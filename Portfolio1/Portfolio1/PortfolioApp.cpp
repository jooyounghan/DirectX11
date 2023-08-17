#include "PortfolioApp.h"
#include "ID3D11Helper.h"

#include <memory>

using namespace std;

PortfolioApp::PortfolioApp(HWND hWindowIn, int& iWidthIn, int& iHeightIn)
	: hMainWindow(hWindowIn),
	iWidth(iWidthIn),
	iHeight(iHeightIn),
	imImgui(iWidth, iHeight),
	pMainCamera(nullptr)
{

}

PortfolioApp::~PortfolioApp()
{

}

void PortfolioApp::Init()
{
	ID3D11Helper::CreateDeviceAndContext(iWidth, iHeight, true, hMainWindow, cpSwapChain.GetAddressOf(), cpDevice.GetAddressOf(), cpDeviceContext.GetAddressOf());
	imImgui.InitImgui(cpDevice.Get(), cpDeviceContext.Get(), hMainWindow);
	ID3D11Helper::SetViewPort(0.f, 0.f, float(iWidth), float(iHeight), 0.f, 1.f, cpDeviceContext.Get(), &screenViewport);

	if (ICamera::DefaultCamera == nullptr)
	{
		ICamera::DefaultCamera = make_shared<ICamera>(cpDevice, cpDeviceContext, cpSwapChain, float(iWidth) / iHeight);
	}
	pMainCamera = ICamera::DefaultCamera.get();
	vModels.push_back(IModel(cpDevice, cpDeviceContext));
}

void PortfolioApp::Update()
{
	pMainCamera->Update();

	for (auto& model : vModels)
	{
		model.Update();
	}
}

void PortfolioApp::Render()
{
	pMainCamera->WipeOut();

	for (auto& model : vModels)
	{
		model.Render();
	}
}

void PortfolioApp::SwapChain()
{
	cpSwapChain->Present(1, 0);
}
