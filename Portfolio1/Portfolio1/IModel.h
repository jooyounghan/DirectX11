#pragma once
#include "ID3D11Helper.h"
#include "StructVar.h"

class IModel
{
public:
	IModel(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~IModel() {};

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

public:
	ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t				ui32IndexCount;

public:
	ComPtr<ID3D11Buffer>	cpVertexBuffer;

public:
	ModelTransform			sModelTransformation;
	ComPtr<ID3D11Buffer>	cpModelMatrixBuffer;

protected:
	ComPtr<ID3D11Device>& cpDevice;
	ComPtr<ID3D11DeviceContext>& cpDeviceContext;
};

