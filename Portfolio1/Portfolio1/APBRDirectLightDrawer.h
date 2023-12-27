#pragma once
#include "DrawerInterface.h"

#include <memory>
#include <vector>

class CameraInterface;
class LightInterface;
class PBRModel;
class PickableModelInterface;

class APBRDirectLightDrawer : public DrawerInterface
{
	friend class MirrorDrawer;

public:
	APBRDirectLightDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~APBRDirectLightDrawer();

protected:
	virtual void SetIAInputLayer() override;
	virtual void SetShader() override;
	virtual void SetOMState() override;
	virtual void ResetDrawer() override;

public:
	void Draw(
		CameraInterface* pCamera,
		LightInterface* pLight,
		const std::vector<std::shared_ptr<PickableModelInterface>> vSpModels
	);

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpBaseInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBaseVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBasePixelShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader>	cpBaseHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>	cpBaseDomainShader;
};