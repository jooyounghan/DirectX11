#pragma once
#include "LightlessDrawer.h"

class ModelInterface;

class ModelOutlineDrawer : public LightlessDrawer
{
public:
	ModelOutlineDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~ModelOutlineDrawer();

public:
	virtual void Draw(CameraInterface* pCamera, ModelInterface* pModel) override;

protected:
	virtual void SetIAInputLayer();

protected:
	virtual void SetShader();

protected:
	virtual void SetOMState();

protected:
	virtual void ResetDrawer();

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		cpBaseInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		cpBaseVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		cpBasePixelShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader>		cpBaseHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		cpBaseDomainShader;
};