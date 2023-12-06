#pragma once
#include "LightlessDrawer.h"

class NormalVectorDrawer : public LightlessDrawer
{
public:
	NormalVectorDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~NormalVectorDrawer();

protected:
	virtual void SetIAInputLayer() override;

protected:
	virtual void SetShader() override;

protected:
	virtual void SetOMState() override;

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
};

