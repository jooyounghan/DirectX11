#include "LightInterface.h"
#include "ID3D11Helper.h"
#include "DefineVar.h"

LightInterface::LightInterface(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn)
{
	AutoZeroMemory(sBaseLightData);
	ID3D11Helper::CreateBuffer(
		pDevice, sBaseLightData, 
		D3D11_USAGE_DEFAULT, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, NULL,
		cpBaseLightDataBuffer.GetAddressOf()
	);
}

LightInterface::~LightInterface()
{
}

void LightInterface::Update()
{
	ID3D11Helper::UpdateBuffer(pDeviceContext, sBaseLightData, D3D11_MAP_WRITE_DISCARD, cpBaseLightDataBuffer.Get());
}
