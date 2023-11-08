#pragma once

#include "NonLightDrawer.h"

class CameraInterface;
class CubeMapModel;

class CubeMapDrawer : public NonLightDrawer
{
public:
	CubeMapDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~CubeMapDrawer();

public:
	void Draw(CameraInterface* pCamera, CubeMapModel* pCubeMap);

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
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpCubeMapInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpCubeMapVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpCubeMapPixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	cpCubeMapSampler;
};
