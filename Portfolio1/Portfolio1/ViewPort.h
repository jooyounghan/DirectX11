#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

class ViewPort
{
private:
	ViewPort(ID3D11DeviceContext* pDeviceContextIn);
	ViewPort(const ViewPort& ref) {}
	ViewPort& operator=(const ViewPort& ref) {}
	~ViewPort() {}

public:
	inline static ViewPort& GetInstance(ID3D11DeviceContext* pDeviceContextIn)
	{
		static ViewPort v = ViewPort(pDeviceContextIn);
		v.pDeviceContext = pDeviceContextIn;
		return v;
	}

public:
	void SetViewPort(
		const float& fTopLeftXIn,
		const float& fTopLeftYIn,
		const float& fWidhtIn,
		const float& fHeightIn,
		const float& fMinDepthIn,
		const float& fMaxDepthIn
	);

private:
	ID3D11DeviceContext* pDeviceContext;
	D3D11_VIEWPORT		 sViewPort;
};

