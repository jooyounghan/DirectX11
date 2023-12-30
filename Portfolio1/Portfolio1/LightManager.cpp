#include "LightManager.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "ID3D11Helper.h"
#include "DefineVar.h"
#include "ShaderTypeEnum.h"

using namespace Microsoft::WRL;

LightManager::LightManager(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn), idxSelectedPointLight(0), idxSelectedSpotLight(0)
{
}

LightManager::~LightManager()
{
	for (size_t idx = 0; idx < vPointLights.size(); ++idx)
	{
		delete vPointLights[idx];
		vPointLights[idx] = nullptr;
	}

	for (size_t idx = 0; idx < vSpotLights.size(); ++idx)
	{
		delete vSpotLights[idx];
		vSpotLights[idx] = nullptr;
	}
}

void LightManager::Update()
{
	for (PointLight* pLight : vPointLights)
	{
		pLight->Update();
	}

	for (SpotLight* pLight : vSpotLights)
	{
		pLight->Update();
	}
}

void LightManager::AddPointLight(
	IN const DirectX::XMVECTOR& xmvLocationIn,
	IN const DirectX::XMVECTOR& xmvLightColorIn,
	IN const float& fFallOffStartIn,
	IN const float& fFallOffEndIn,
	IN const float& fLightPowerIn
)
{
	PointLight* pPointLight = new PointLight(
		pDevice, pDeviceContext, 
		xmvLocationIn, xmvLightColorIn, 
		fFallOffStartIn, fFallOffEndIn, 
		fLightPowerIn
	);
	vPointLights.push_back(pPointLight);
}

void LightManager::AddSpotLight(
	IN const DirectX::XMVECTOR& xmvLocationIn,
	IN const DirectX::XMVECTOR& xmvDirectionIn,
	IN const DirectX::XMVECTOR& xmvLightColorIn,
	IN const float& fFallOffStartIn,
	IN const float& fFallOffEndIn,
	IN const float& fLightPowerIn,
	IN const float& fSpotPowerIn
)
{
	SpotLight* pSpotLight = new SpotLight(
		pDevice, pDeviceContext, 
		xmvLocationIn, xmvDirectionIn, 
		xmvLightColorIn, fFallOffStartIn, 
		fFallOffEndIn, fLightPowerIn, 
		fSpotPowerIn
	);
	vSpotLights.push_back(pSpotLight);
}
