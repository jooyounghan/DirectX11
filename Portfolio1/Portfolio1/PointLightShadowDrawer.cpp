#include "PointLightShadowDrawer.h"
#include "PointLight.h"
#include "PickableModelInterface.h"

PointLightShadowDrawer::PointLightShadowDrawer(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: AShadowDrawer(pDeviceIn, pDeviceContextIn)
{
}

PointLightShadowDrawer::~PointLightShadowDrawer()
{
}

void PointLightShadowDrawer::Draw(
	LightInterface* pLightInterface,
	const std::vector<std::shared_ptr<PickableModelInterface>> spSelectedModels
)
{
	SetIAInputLayer();
	SetShader();
	SetOMState();

	PointLight* pPointLight = (PointLight*)pLightInterface;
	
	for (size_t idx = 0; idx < PointLightViewProj::PointViewProjNum; ++idx)
	{
		pPointLight->WipeOut(PointLightViewProj::XViewProj + idx);
		pPointLight->SetConstantBuffers(PointLightViewProj::XViewProj + idx);
		pPointLight->OMSetRenderTarget(PointLightViewProj::XViewProj + idx);
		for (auto& pModel : spSelectedModels)
		{
			pModel->SetIAProperties();
			pModel->SetConstantBuffers();

			pModel->Render();

			pModel->ResetConstantBuffers();
		}
		pPointLight->ResetConstantBuffers();
	}

	ResetDrawer();
}