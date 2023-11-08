#pragma once
#include "NonLightDrawer.h"

class NormalVectorDrawer : public NonLightDrawer
{
public:
	NormalVectorDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~NormalVectorDrawer();

protected:
	virtual void SetIAInputLayer() override;
	virtual void SetVSShader() override;
	virtual void SetHSShader() override;
	virtual void SetDSShader() override;
	virtual void SetGSShader() override;
	virtual void SetPSShader() override;

protected:
	virtual void SetOMState() override;
	virtual void ResetOMState() override;

protected:
	virtual void ResetDrawer() override;

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

