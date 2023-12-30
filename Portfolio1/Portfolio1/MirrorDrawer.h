#pragma once
#include "DrawerInterface.h"

#include <memory>
#include <vector>

class PBRModel;
class CameraInterface;
class LightInterface;
class CubeMapDrawer;
class CubeMapModel;
class MirrorModel;

class MirrorDrawer : public DrawerInterface
{
public:
	MirrorDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~MirrorDrawer();

public:
	void Draw(
		CameraInterface* pCamera,
		LightInterface* pLight,
		class APBRDirectLightDrawer* aPBRDirectLightingDrawer,
		const std::vector<std::shared_ptr<PBRModel>>& vSpModels,
		CubeMapDrawer* pCubeMapDrawer,
		CubeMapModel* pEnvironmentCubeMap,
		const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModels
	);

protected:
	virtual void SetIAInputLayer() override;

protected:
	virtual void SetShader() override;

protected:
	virtual void SetOMState() override;

protected:
	virtual void ResetDrawer() override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpMirrorInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpMirrorVertexShader;


protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpMirrorPixelShader;
};