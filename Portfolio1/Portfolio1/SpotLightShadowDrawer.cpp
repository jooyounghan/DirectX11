#include "SpotLightShadowDrawer.h"
#include "SpotLight.h"
#include "PickableModelInterface.h"

SpotLightShadowDrawer::SpotLightShadowDrawer(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn
)
	: AShadowDrawer(pDeviceIn, pDeviceContextIn)
{
}

SpotLightShadowDrawer::~SpotLightShadowDrawer()
{
}

void SpotLightShadowDrawer::Draw(
	LightInterface* pLightInterface, 
	const std::vector<std::shared_ptr<PickableModelInterface>> spSelectedModels
)
{
	SetIAInputLayer();
	SetShader();
	SetOMState();

	SpotLight* pSpotLight = (SpotLight*)pLightInterface;
	pSpotLight->SetConstantBuffers();
	pSpotLight->OMSetRenderTarget();
	for (auto& pModels : spSelectedModels)
	{
		pModels->Render();
	}
	pSpotLight->ResetConstantBuffers();

	ResetDrawer();
}
