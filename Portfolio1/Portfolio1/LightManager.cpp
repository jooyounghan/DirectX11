#include "LightManager.h"
#include "ID3D11Helper.h"
#include "DefineVar.h"
#include "ObjectModelEnum.h"

using namespace Microsoft::WRL;

LightSet LightManager::sTempLightSet;

LightManager::LightManager(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), usSelectedLightIndex(0)
{
	vLights.resize(ullMaxLightNum);
	memset(vLights.data(), 0, vLights.size() * sizeof(LightSet));
	ID3D11Helper::CreateBuffer(cpDevice.Get(), vLights, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, NULL, cpLightsConstantBuffer.GetAddressOf());
	vLights.clear();
}

LightManager::~LightManager()
{
}

const unsigned short& LightManager::GetSelectedLightIndex() {
	return usSelectedLightIndex;
}

void LightManager::SetSelectedLightIndex(const unsigned short& index)
{
	usSelectedLightIndex = index;
}

const std::vector<LightSet>& LightManager::GetLights()
{
	return vLights;
}

void LightManager::AddDirectionalLight(
	IN const DirectX::XMVECTOR& xmvLocationIn,
	IN const DirectX::XMVECTOR& xmvLightColorIn,
	IN const DirectX::XMVECTOR& xmvDirectionIn)
{
	LightSet lightSet;
	AutoZeroMemory(lightSet);
	lightSet.eLightType = LightType::Directional;
	lightSet.xmvLocation = xmvLocationIn;
	lightSet.xmvLightColor = xmvLightColorIn;
	lightSet.xmvDirection = xmvDirectionIn;
	vLights.push_back(lightSet);
}

void LightManager::AddPointLight()
{
}

void LightManager::AddSpotLight()
{
}

void LightManager::Update()
{
	D3D11_MAPPED_SUBRESOURCE ms;
	cpDeviceContext->Map(cpLightsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vLights.data(), sizeof(LightSet) * vLights.size());
	cpDeviceContext->Unmap(cpLightsConstantBuffer.Get(), 0);
}

void LightManager::SetVSConstantBuffers()
{
}

void LightManager::SetHSConstantBuffers()
{
}

void LightManager::SetDSConstantBuffers()
{
}

void LightManager::SetPSConstantBuffers()
{
	cpDeviceContext->PSSetConstantBuffers(ObjectPSConstBufferType::OBJECT_PS_LIGHT, 1, cpLightsConstantBuffer.GetAddressOf());
}

void LightManager::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->PSSetConstantBuffers(ObjectPSConstBufferType::OBJECT_PS_LIGHT, 1, &pResetBuffer);
}
