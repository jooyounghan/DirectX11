#include "IModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

uint32_t IModel::uiGlobalModelID = 1;

IModel::IModel() 
{
	AutoZeroMemory(sModelData);
	sModelData.uiModelID = IModel::uiGlobalModelID++;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sModelData,
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_CONSTANT_BUFFER, 
		NULL, NULL, cpIdBuffer.GetAddressOf()
	);
}

IModel::~IModel() {}
