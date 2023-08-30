#include "ILight.h"
#include "ID3D11Helper.h"
#include "EnumVar.h"

using namespace std;
using Microsoft::WRL::ComPtr;

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
}

void ILight::UpdateLights(ID3D11DeviceContext* pDeviceContext)
{
	for (auto& ls : vLightSets)
	{
		ls.xmvLightColor.m128_f32[1] += 0.01;
	}
	ID3D11Helper::UpdateBuffer(pDeviceContext, vLightSets, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, cpLightConstantBuffer.Get());
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::LightStructure, 1, cpLightConstantBuffer.GetAddressOf());
}