#pragma once
#include "DrawerInterface.h"

#include <memory>
#include <vector>

class ObjectModel;
class CameraInterface;
class LightManager;
class CubeMapModel;
class MirrorModel;

class MirrorDrawer : protected DrawerInterface
{
public:
	MirrorDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~MirrorDrawer();

public:
	void Draw(
		class ObjectDrawer* pModelDrawer,
		class CubeMapDrawer* pCubeMapDrawer,
		CameraInterface* pCamera,
		LightManager* pLightManager,
		const std::vector<std::shared_ptr<ObjectModel>>& vSpModels,
		CubeMapModel* pEnvironmentCubeMap,
		const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModels
	);
	
	void RenderMirror(
		CameraInterface* pCamera,
		const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModel
	);

public:
	virtual void SetIAInputLayer() override;
	virtual void SetVSShader() override;
	virtual void SetHSShader() override;
	virtual void SetDSShader() override;
	virtual void SetGSShader() override;
	virtual void SetPSShader() override;

public:
	virtual void SetOMState() override;
	virtual void ResetOMState() override;

public:
	virtual void ResetDrawer() override;

public:
	void PresetConfig(CameraInterface* pCamera);
	void ResetConfig(CameraInterface* pCamera);

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpMirrorInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpMirrorVertexShader;


protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpMirrorPixelShader;
};