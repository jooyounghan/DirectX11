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
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		cpNVInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		cpNVVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	cpNVGeometryShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		cpNVPixelShader;
};

