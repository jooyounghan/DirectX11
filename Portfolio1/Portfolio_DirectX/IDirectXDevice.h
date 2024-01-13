#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath/DirectXMath.h>

class IDirectXDevice
{
public:
	IDirectXDevice(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	~IDirectXDevice();

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
};

