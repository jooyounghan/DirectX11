#pragma once
#include "DrawerInterface.h"

#include <vector>
#include <memory>

enum ELightType : UINT;
class LightInterface;
class PickableModelInterface;

class AShadowDrawer : public DrawerInterface
{
public:
	AShadowDrawer(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~AShadowDrawer();

public:
	virtual void Draw(
		LightInterface* pLightInterface,
		const std::vector<std::shared_ptr<PickableModelInterface>> spSelectedModels
	) = 0;

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

