#pragma once
#include "DrawerInterface.h"

class BaseModelDrawer : public DrawerInterface
{
template<typename Drawer>
friend class Canvas;

public:
	BaseModelDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~BaseModelDrawer();

protected:
	void SetIAInputLayer();
	void SetVSShader();
	void SetGSShader();
	void SetHSShader();
	void SetDSShader();
	void SetPSShader();

protected:
	void SetOMState();
	void ResetOMState();

protected:
	void ResetDrawer();

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