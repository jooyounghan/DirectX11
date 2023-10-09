#pragma once
#include "DrawerInterface.h"

class NormalVectorDrawer : public DrawerInterface
{
	template<typename Drawer>
	friend class Canvas;

public:
	NormalVectorDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~NormalVectorDrawer();

protected:
	void SetIAInputLayer();
	void SetVSShader();
	void SetHSShader();
	void SetDSShader();
	void SetGSShader();
	void SetPSShader();

protected:
	void SetOMState();
	void ResetOMState();

protected:
	void ResetDrawer();

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		cpNVInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		cpNVVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader>		cpNVHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		cpNVDomainShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	cpNVGeometryShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		cpNVPixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		cpNVSampler;
};

