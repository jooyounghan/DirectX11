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
Drawer�� ��� ���� ��å �������� ���Ͽ� �����Լ� ȣ���� �ּ�ȭ�Ͽ���
Model�� ��� �������� ���Ͽ� �����Ͽ���.

�� Drawer�� ��� ��ӿ� ���� �ʿ伺�� ��������,
�� Model�� ��� ��ӿ� ���� �ʿ伺�� �־���.

���� ��� Drawer�� �� �׸����� �ϴ� ������� ��� ������ �ұ�Ģ������ �ٸ��� ������ ������,
Model�� �⺻������ ������ �־�� �ϴ� ��� ������ �����ϱ� ������.
*/