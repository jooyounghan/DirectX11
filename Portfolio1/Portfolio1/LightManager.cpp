#include "LightManager.h"

#include "PointLight.h"
#include "SpotLight.h"

#include "ID3D11Helper.h"
#include "DefineVar.h"
#include "ShaderTypeEnum.h"

using namespace Microsoft::WRL;

LightManager::LightManager(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn), idxSelectedLight(0)
{
}

LightManager::~LightManager()
{
	for (size_t idx = 0; idx < vLights.size(); ++idx)
	{
		delete vLights[idx];
		vLights[idx] = nullptr;
	}
}

const size_t& LightManager::GetSelectedLightIndex() {
	return idxSelectedLight;
}

void LightManager::SetSelectedLightIndex(const size_t& index)
{
	idxSelectedLight = index;
}

void LightManager::AddPointLight(
	IN const DirectX::XMVECTOR& xmvLocationIn,
	IN const float* pLightColorIn,
	IN const float& fFallOffStartIn,
	IN const float& fFallOffEndIn,
	IN const float& fLightPowerIn
)
{
	PointLight* pPointLight = new PointLight(pDevice, pDeviceContext, xmvLocationIn, pLightColorIn, fFallOffStartIn, fFallOffEndIn, fLightPowerIn);
	vLights.push_back((LightInterface*)pPointLight);
}

void LightManager::AddSpotLight(
	IN const DirectX::XMVECTOR& xmvLocationIn,
	IN const DirectX::XMVECTOR& xmvDirectionIn,
	IN const float* pLightColorIn,
	IN const float& fFallOffStartIn,
	IN const float& fFallOffEndIn,
	IN const float& fLightPowerIn,
	IN const float& fSpotPowerIn
)
{
	SpotLight* pSpotLight = new SpotLight(pDevice, pDeviceContext, xmvLocationIn, xmvDirectionIn, pLightColorIn, fFallOffStartIn, fFallOffEndIn, fLightPowerIn, fSpotPowerIn);
	vLights.push_back((LightInterface*)pSpotLight);
}
