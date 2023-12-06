#pragma once

#include "LightlessDrawer.h"

class CameraInterface;
class CubeMapModel;

class CubeMapDrawer : public LightlessDrawer
{
	friend class MirrorDrawer;

public:
	CubeMapDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~CubeMapDrawer();

protected:
	virtual void SetIAInputLayer() override;

protected:
	virtual void SetShader() override;

protected:
	virtual void SetOMState() override;

protected:
	virtual void ResetDrawer() override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpCubeMapInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpCubeMapVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpCubeMapPixelShader;
};
