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
	pSpotLight->WipeOut();
	pSpotLight->SetConstantBuffers();
	pSpotLight->OMSetRenderTarget();

	for (auto& pModel : spSelectedModels)
	{
		pModel->SetIAProperties();
		pModel->SetConstantBuffers();

		pModel->Render();

		pModel->ResetConstantBuffers();
	}
	pSpotLight->ResetConstantBuffers();

	ResetDrawer();
}
