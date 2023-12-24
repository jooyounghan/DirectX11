#pragma once
#include "DrawerInterface.h"

#include <memory>
#include <vector>

class CameraInterface;
class LightManager;
class PBRModel;
class PickableModelInterface;

class PBRDirectLightingDrawer : public DrawerInterface
{
	friend class MirrorDrawer;

public:
	PBRDirectLightingDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn);
	virtual ~PBRDirectLightingDrawer();

public:
	void Draw(
		CameraInterface* pCamera,
		LightManager* pLightManager,
		const std::vector<std::shared_ptr<PBRModel>> vSpModels
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
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpBaseInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBaseVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBaseDLPS;

protected:
	Microsoft::WRL::ComPtr<ID3D11HullShader>	cpBaseHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>	cpBaseDomainShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpOutlinerPixelShader;
};