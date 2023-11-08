#pragma once
#include "NonLightDrawer.h"

class ModelInterface;

class ModelOutlineDrawer : public NonLightDrawer
{
public:
	ModelOutlineDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~ModelOutlineDrawer();

public:
	virtual void Draw(CameraInterface* pCamera, ModelInterface* pModel) override;

protected:
	virtual void SetIAInputLayer();
	virtual void SetVSShader();
	virtual void SetHSShader();
	virtual void SetDSShader();
	virtual void SetGSShader();
	virtual void SetPSShader();

protected:
	virtual void SetOMState();
	virtual void ResetOMState();

protected:
	virtual void ResetDrawer();

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		cpBaseInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		cpBaseVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		cpBasePixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		cpBaseSampler;

protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader>		cpBaseHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		cpBaseDomainShader;
};