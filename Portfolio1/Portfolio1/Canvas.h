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
Drawer�� ��� ���� ��å �������� ���Ͽ� �����Լ� ȣ���� �ּ�ȭ�Ͽ���
Model�� ��� �������� ���Ͽ� �����Ͽ���.

�� Drawer�� ��� ��ӿ� ���� �ʿ伺�� ��������,
�� Model�� ��� ��ӿ� ���� �ʿ伺�� �־���.

���� ��� Drawer�� �� �׸����� �ϴ� ������� ��� ������ �ұ�Ģ������ �ٸ��� ������ ������,
Model�� �⺻������ ������ �־�� �ϴ� ��� ������ �����ϱ� ������.
*/