#pragma once
#include "DrawerInterface.h"

#include <memory>
#include <vector>

class PBRModel;
class CameraInterface;
class LightManager;
class CubeMapModel;
class MirrorModel;

class MirrorDrawer : protected DrawerInterface
{
public:
	MirrorDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~MirrorDrawer();

public:
	void Draw(
		CameraInterface* pCamera,
		LightManager* pLightManager,
		class PBRSpotLightDrawer* pPBRDirectLightingDrawer,
		const std::vector<std::shared_ptr<PBRModel>>& vSpModels,
		class CubeMapDrawer* pCubeMapDrawer,
		CubeMapModel* pEnvironmentCubeMap,
		const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModels
	);
	
	void RenderMirror(
		CameraInterface* pCamera,
		const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModel
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