#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

template <typename Drawer, typename Model>
class Canvas
{
public:
	Canvas(Drawer& drawerIn, Model& modelIn);

public:
	void Render();

public:
	Drawer& drawer;
	Model& model;
};

template<typename Drawer, typename Model>
Canvas<Drawer, Model>::Canvas(Drawer& drawerIn, Model& modelIn)
	: drawer(drawerIn), model(modelIn)
{
}

template<typename Drawer, typename Model>
void Canvas<Drawer, Model>::Render()
{
	drawer.SetIAInputLayer();
	model.SetIAProperties();

	drawer.SetVSShader();
	model.SetVSConstantBuffer();
	model.SetVSShaderResources();

	drawer.SetHSShader();
	model.SetHSConstantBuffer();
	model.SetHSShaderResources();

	drawer.SetDSShader();
	model.SetDSConstantBuffer();
	model.SetDSShaderResources();

	drawer.SetPSShader();
	model.SetPSConstantBuffer();
	model.SetPSShaderResources();

	model.SetOM();
	model.Render();
	model.ResetOM();

	drawer.ResetDrawer();
};
