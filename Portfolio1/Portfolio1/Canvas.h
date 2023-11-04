#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

template <typename Drawer>
class Canvas
{
public:
	Canvas();

public:
	inline void Render();

public:
	inline void SetDrawer(Drawer* drawerIn);

protected:
	Drawer* drawer;
};

template<typename Drawer>
Canvas<Drawer>::Canvas()
{
}

template<typename Drawer>
inline void Canvas<Drawer>::SetDrawer(Drawer* drawerIn)
{
	drawer = drawerIn;
}

template<typename Drawer>
inline void Canvas<Drawer>::Render()
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

		drawer->pCamera->SetRSState();
		drawer->pCamera->SetVSConstantBuffers();
		drawer->pCamera->SetGSConstantBuffers();
		drawer->pCamera->SetHSConstantBuffers();
		drawer->pCamera->SetDSConstantBuffers();
		drawer->pCamera->SetPSConstantBuffers();
		drawer->pCamera->OMSetRenderTargets();

		if (drawer->pLightManager)
		{
			drawer->pLightManager->SetPSConstantBuffers();
		}

		drawer->SetOMState();
		drawer->pModel->Render();
		drawer->ResetOMState();

		drawer->pCamera->ResetCamera();

		drawer->pModel->ResetConstantBuffers();
		drawer->pModel->ResetShaderResources();

		drawer->ResetDrawer();
	}
}