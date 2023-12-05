#pragma once

#include "NonLightDrawer.h"

class CameraInterface;
class CubeMapModel;

class CubeMapDrawer : public NonLightDrawer
{
	friend class MirrorDrawer;

public:
	CubeMapDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~CubeMapDrawer();

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
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpCubeMapInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpCubeMapVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpCubeMapPixelShader;
};
