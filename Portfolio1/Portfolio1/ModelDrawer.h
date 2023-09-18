#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <mutex>
#include <atomic>

template<typename Model>
class ModelDrawerImpl;

template<typename Model>
class ModelDrawer
{
public:
	template<typename ...Args>
	ModelDrawer(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, Args... args);
	~ModelDrawer() {};

public:
	void Update();
	void Render();

protected:
	ModelDrawerImpl<Model> templateModelImpl;
};

template<typename Model>
template<typename ...Args>
ModelDrawer<Model>::ModelDrawer(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn, Args ...args)
	: templateModelImpl(args...)
{

}

template<typename Model>
inline void ModelDrawer<Model>::Update()
{
	return templateModelImpl.Update();
}

template<typename Model>
inline void ModelDrawer<Model>::Render()
{
	return templateModelImpl.Render();
}