#pragma once
#include "BaseModelDrawer.h"

#include <memory>
#include <vector>

class PickableModel;
class CameraInterface;
class LightManager;
class CubeMapModel;
class MirrorModel;

class MirrorDrawer : protected BaseModelDrawer
{
public:
	MirrorDrawer(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	virtual ~MirrorDrawer();

public:
	void Draw(
		CameraInterface* pCamera,
		LightManager* pLightManager,
		const std::vector<std::shared_ptr<PickableModel>>& vSpModels,
		CubeMapModel* pEnvironmentCubeMap,
		const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModel
	);

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpMirrorPixelShader;
};