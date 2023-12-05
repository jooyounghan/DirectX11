#pragma once
#include "ObjectDrawer.h"

class BaseModelDrawer : public ObjectDrawer
{
public:
	BaseModelDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~BaseModelDrawer();

public:
	virtual void SetIAInputLayer() override;
	virtual void SetVSShader() override;
	virtual void SetHSShader() override;
	virtual void SetDSShader() override;
	virtual void SetGSShader() override;
	virtual void SetPSShader() override;

public:
	virtual void SetOMState() override;
	virtual void ResetOMState() override;

public:
	virtual void ResetDrawer() override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpBaseInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBaseVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBasePixelShader;


protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader>	cpBaseHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>	cpBaseDomainShader;
};