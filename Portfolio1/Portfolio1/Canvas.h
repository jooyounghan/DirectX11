#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

template <typename Drawer>
class Canvas
{
public:
	Canvas(Drawer* drawerIn);

public:
	void Render();

protected:
	Drawer* drawer;
};

template<typename Drawer>
Canvas<Drawer>::Canvas(Drawer* drawerIn)
	: drawer(drawerIn)
{
}

template<typename Drawer>
void Canvas<Drawer>::Render()
{
	if (drawer->pModel)
	{
		drawer->SetIAInputLayer();
		drawer->SetVSShader();
		drawer->SetGSShader();
		drawer->SetHSShader();
		drawer->SetDSShader();
		drawer->SetPSShader();

		drawer->pModel->SetIAProperties();
		drawer->pModel->SetVSConstantBuffers();
		drawer->pModel->SetVSShaderResources();
		drawer->pModel->SetGSConstantBuffers();
		drawer->pModel->SetGSShaderResources();
		drawer->pModel->SetHSConstantBuffers();
		drawer->pModel->SetHSShaderResources();
		drawer->pModel->SetDSConstantBuffers();
		drawer->pModel->SetDSShaderResources();
		drawer->pModel->SetPSConstantBuffers();
		drawer->pModel->SetPSShaderResources();

		if (drawer->pCamera)
		{
			drawer->pCamera->SetRSState();
			drawer->pCamera->SetVSConstantBuffers();
			drawer->pCamera->SetGSConstantBuffers();
			drawer->pCamera->SetHSConstantBuffers();
			drawer->pCamera->SetDSConstantBuffers();
			drawer->pCamera->SetPSConstantBuffers();
			drawer->pCamera->OMSetRenderTargets();
		}

		if (drawer->pLightManager)
		{
			drawer->pLightManager->SetPSConstantBuffers();
		}

		drawer->SetOMState();
		drawer->pModel->Render();
		drawer->ResetOMState();

		drawer->ResetDrawer();
	}
};

/*
Drawer의 경우 단위 정책 디자인을 통하여 가상함수 호출을 최소화하였고
Model의 경우 다형성을 통하여 구현하였음.

각 Drawer의 경우 상속에 대한 필요성이 떨어졌고,
각 Model의 경우 상속에 대한 필요성이 있었음.

예를 들어 Drawer는 각 그리고자 하는 방식으로 멤버 변수를 불규칙적으로 다르게 가지고 있지만,
Model은 기본적으로 가지고 있어야 하는 멤버 변수가 동일하기 때문임.
*/