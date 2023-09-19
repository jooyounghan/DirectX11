#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

#include <mutex>
#include <memory>
#include <atomic>

#include <vector>

#include "ID3D11Helper.h"

class ModelDrawer
{
template<typename Drawer, typename Model>
friend class Canvas;

public:
	ModelDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~ModelDrawer();

public:
	void SetIAInputLayer();
	void SetVSShader();
	void SetHSShader();
	void SetDSShader();
	void SetPSShader();

public:
	void ResetDrawer();

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>&			cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>&	cpDeviceContext;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		cpBaseInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBaseVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		cpBasePixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	cpBaseSampler;

protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader>		cpBaseHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>	cpBaseDomainShader;
};