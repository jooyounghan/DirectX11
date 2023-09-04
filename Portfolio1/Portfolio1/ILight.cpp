#include "ILight.h"
#include "ID3D11Helper.h"
#include "EnumVar.h"

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

LightSet				ILight::sTempLightSet;
size_t					ILight::ullNextLightId = 0;
map<size_t, LightSet*>	ILight::mIdToLightSet;
vector<LightSet>		ILight::vLightSets;
ComPtr<ID3D11Buffer>	ILight::cpLightConstantBuffer;


ILight::ILight(
	ComPtr<ID3D11Device>& cpDeviceIn,
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), ullLightId(ullNextLightId)
{
	ullNextLightId++;
}

ILight::~ILight()
{
}

void ILight::InitLights(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	vLightSets.resize(ullMaxLightNum);
	ID3D11Helper::CreateBuffer(
		pDevice,
		vLightSets,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, 0,
		cpLightConstantBuffer.GetAddressOf()
	);
	vLightSets.clear();
}

void ILight::UpdateLights(ID3D11DeviceContext* pDeviceContext)
{
	ID3D11Helper::UpdateBuffer(pDeviceContext, vLightSets, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, cpLightConstantBuffer.Get());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::LightStructure, 1, cpLightConstantBuffer.GetAddressOf());
}

LightSet* ILight::GetLightSet()
{
	LightSet* pResult = nullptr;
	if (mIdToLightSet.find(ullLightId) != mIdToLightSet.end())
	{
		pResult = mIdToLightSet[ullLightId];
	}
	return pResult;
}

LightType ILight::GetLightType()
{
	if (mIdToLightSet.find(ullLightId) != mIdToLightSet.end())
	{
		return mIdToLightSet[ullLightId]->eLightType;
	}
	else
	{
		Console("ID에 해당하는 라이트 정보를 얻지 못했습니다.");
		return LightType::NotALight;
	}
}

void ILight::AddLightSet()
{
	vLightSets.emplace_back();
	LightSet* pLightSet = &vLightSets.back();
	mIdToLightSet.emplace(ullLightId, pLightSet);
}

void ILight::RemoveLightSet()
{
	LightSet* pRemoveLightSet = mIdToLightSet.at(ullLightId);

	mIdToLightSet.erase(ullLightId);
	vLightSets.erase(remove_if(vLightSets.begin(), vLightSets.end(),
		[&](LightSet& pTempLightSet) { return &pTempLightSet == pRemoveLightSet; }), vLightSets.end());
}

LightSet* ILight::GetTempLightSet()
{
	return &sTempLightSet;
}