#include "APickable.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

APickable::APickable()
{
	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice, 1, 1,
		1, 0, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_READ,
		NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
		cpPickedIDTexture.GetAddressOf()
	);
	ID3D11Helper::CreateUnorderedAccessView(DirectXDevice::pDevice, cpPickedIDTexture.Get(), cpPickedIDUAV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpPickedIDTexture.Get(), cpPickedIDSRV.GetAddressOf());

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sMousePosNdc,
		D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, NULL,
		cpMousePosNdc.GetAddressOf()
	);
}

APickable::~APickable()
{
}

uint32_t APickable::GetPickedID()
{
	uint32_t uiResult = 0;
	D3D11_MAPPED_SUBRESOURCE sMappedSubResource;
	AutoZeroMemory(sMappedSubResource);
	DirectXDevice::pDeviceContext->Map(cpPickedIDTexture.Get(), 0, D3D11_MAP_READ, NULL, &sMappedSubResource);
	memcpy(&uiResult, sMappedSubResource.pData, sizeof(uint32_t));
	DirectXDevice::pDeviceContext->Unmap(cpPickedIDTexture.Get(), 0);
	return uiResult;
}
