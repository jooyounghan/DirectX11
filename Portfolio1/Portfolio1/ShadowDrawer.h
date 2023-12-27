#pragma once
#include "DrawerInterface.h"

#include <vector>
#include <memory>

enum ELightType : size_t;
class LightInterface;
class PickableModelInterface;

class ShadowDrawer : public DrawerInterface
{
public:
	ShadowDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~ShadowDrawer();

public:
	void Draw(
		LightInterface* pLightInterface,
		const std::vector<std::shared_ptr<PickableModelInterface>> spSelectedModels
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
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpShadowInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpShadowVertexShader;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpShadowPixelShader;
};

