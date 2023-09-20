#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

class ModelInterface;

class ModelOutlineDrawer
{
	template<typename Drawer>
	friend class Canvas;

public:
	ModelOutlineDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~ModelOutlineDrawer();

protected:
	void SetIAInputLayer();
	void SetVSShader();
	void SetHSShader();
	void SetDSShader();
	void SetPSShader();

protected:
	void SetOM();
	void ResetOM();

protected:
	void ResetDrawer();

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>&			cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>&	cpDeviceContext;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		cpBaseInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		cpBaseVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		cpBasePixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		cpBaseSampler;

protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader>		cpBaseHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		cpBaseDomainShader;

protected:
	ModelInterface* pModel;

public:
	void SetModel(ModelInterface* modelInterface);
};