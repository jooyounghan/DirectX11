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
	drawer->SetIAInputLayer();
	drawer->pModel->SetIAProperties();

	drawer->SetVSShader();
	drawer->pModel->SetVSConstantBuffer();
	drawer->pModel->SetVSShaderResources();

	drawer->SetHSShader();
	drawer->pModel->SetHSConstantBuffer();
	drawer->pModel->SetHSShaderResources();

	drawer->SetDSShader();
	drawer->pModel->SetDSConstantBuffer();
	drawer->pModel->SetDSShaderResources();

	drawer->SetPSShader();
	drawer->pModel->SetPSConstantBuffer();
	drawer->pModel->SetPSShaderResources();

	drawer->SetOM();
	drawer->pModel->Render();
	drawer->ResetOM();

	drawer->ResetDrawer();
};

/*
Drawer의 경우 단위 정책 디자인을 통하여 가상함수 호출을 최소화하였고
Model의 경우 다형성을 통하여 구현하였음.

각 Drawer의 경우 상속에 대한 필요성이 떨어졌고,
각 Model의 경우 상속에 대한 필요성이 있었음.

예를 들어 Drawer는 각 그리고자 하는 방식으로 멤버 변수를 불규칙적으로 다르게 가지고 있지만,
Model은 기본적으로 가지고 있어야 하는 멤버 변수가 동일하기 때문임.
*/