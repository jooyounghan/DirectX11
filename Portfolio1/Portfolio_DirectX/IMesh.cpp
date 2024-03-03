#include "IMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

uint32_t IMesh::uiGlobalModelID = 1;

IMesh::IMesh() 
{
	AutoZeroMemory(sModelData);
	sModelData.uIMeshID = IMesh::uiGlobalModelID++;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sModelData,
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_CONSTANT_BUFFER, 
		NULL, NULL, cpIdBuffer.GetAddressOf()
	);
}

IMesh::~IMesh() {}
