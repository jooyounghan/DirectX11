#include "LightManager.h"
#include "ID3D11Helper.h"
#include "DefineVar.h"
#include "ShaderTypeEnum.h"

using namespace Microsoft::WRL;

LightSet LightManager::sTempLightSet;

LightManager::LightManager(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), idxSelectedLight(0)
{
	vLights.resize(ullMaxLightNum);
	memset(vLights.data(), 0, vLights.size() * sizeof(LightSet));
	ID3D11Helper::CreateBuffer(cpDevice.Get(), vLights, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, NULL, cpLightsConstantBuffer.GetAddressOf());
	vLights.clear();
}

LightManager::~LightManager()
{
}

const size_t& LightManager::GetSelectedLightIndex() {
	return idxSelectedLight;
}

void LightManager::SetSelectedLightIndex(const size_t& index)
{
	idxSelectedLight = index;
}

const std::vector<LightSet>& LightManager::GetLights()
{
	return vLights;
}

void LightManager::AddDirectionalLight(
	IN const DirectX::XMVECTOR& xmvLightColorIn,
	IN const DirectX::XMVECTOR& xmvDirectionIn,
	const float& fLightPowerIn
)
{
	LightSet lightSet;
	AutoZeroMemory(lightSet);
	lightSet.eLightType = LightType::Directional;
	lightSet.xmvLightColor = xmvLightColorIn;
	lightSet.xmvDirection = xmvDirectionIn;
	lightSet.fLightPower = fLightPowerIn;
	vLights.push_back(lightSet);
}

void LightManager::AddPointLight(
	IN const DirectX::XMVECTOR& xmvLocationIn,
	IN const DirectX::XMVECTOR& xmvLightColorIn,
	IN const float& fFallOffStart,
	IN const float& fFallOffEnd,
	const float& fLightPowerIn
)
{
	LightSet lightSet;
	AutoZeroMemory(lightSet);
	lightSet.eLightType = LightType::Point;
	lightSet.xmvLocation = xmvLocationIn;
	lightSet.xmvLightColor = xmvLightColorIn;
	lightSet.fFallOffStart = fFallOffStart;
	lightSet.fFallOffEnd = fFallOffEnd;
	lightSet.fLightPower = fLightPowerIn;
	vLights.push_back(lightSet);
}

void LightManager::AddSpotLight(
	IN const DirectX::XMVECTOR& xmvLocationIn,
	IN const DirectX::XMVECTOR& xmvLightColorIn,
	IN const float& fFallOffStart,
	IN const float& fFallOffEnd,
	IN const float& fLightPowerIn,
	IN const float& fSpotPower
)
{
	LightSet lightSet;
	AutoZeroMemory(lightSet);
	lightSet.eLightType = LightType::Spot;
	lightSet.xmvLocation = xmvLocationIn;
	lightSet.xmvLightColor = xmvLightColorIn;
	lightSet.fFallOffStart = fFallOffStart;
	lightSet.fFallOffEnd = fFallOffEnd;
	lightSet.fLightPower = fLightPowerIn;
	lightSet.fSpotPower = fSpotPower;
	vLights.push_back(lightSet);
}

void LightManager::Update()
{
	D3D11_MAPPED_SUBRESOURCE ms;
	cpDeviceContext->Map(cpLightsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vLights.data(), sizeof(LightSet) * vLights.size());
	cpDeviceContext->Unmap(cpLightsConstantBuffer.Get(), 0);
}

void LightManager::SetConstantBuffers()
{
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_LIGHTBASE, 1, cpLightsConstantBuffer.GetAddressOf());
}

void LightManager::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_LIGHTBASE, 1, &pResetBuffer);
}
