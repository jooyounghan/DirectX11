#include "IObject.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

uint32_t IObject::uiGlobalObjectID = 1;

IObject::IObject()
{
	AutoZeroMemory(sObjectData);
	sObjectData.uIObjectID = IObject::uiGlobalObjectID++;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sObjectData,
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_CONSTANT_BUFFER,
		NULL, NULL, cpObjectBuffer.GetAddressOf()
	);
}

IObject::~IObject()
{
}
