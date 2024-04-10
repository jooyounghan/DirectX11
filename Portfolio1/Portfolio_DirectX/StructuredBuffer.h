#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <vector>

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

template<typename T>
class StructuredBuffer : public std::vector<T>
{
public:
	StructuredBuffer(const size_t& uiNumData, const T& initialValue);
	virtual ~StructuredBuffer();

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpStagingBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpStructuredBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpStructuredSRV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> cpStructuredUAV;

public:
	inline ID3D11ShaderResourceView* const* GetStructuredSRV() { return cpStructuredSRV.GetAddressOf(); }
	inline ID3D11UnorderedAccessView* const* GetStructuredUAV() { return cpStructuredUAV.GetAddressOf(); }

public:
	void UpdateStructuredBuffer();
};

template<typename T>
inline StructuredBuffer<T>::StructuredBuffer(const size_t& uiNumData, const T& initialValue)
{
	this->resize(uiNumData, initialValue);
	
	ID3D11Helper::CreateStagingBuffer(
		DirectXDevice::pDevice,
		*this,
		cpStagingBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateStructuredBuffer(
		DirectXDevice::pDevice,
		*this,
		cpStructuredBuffer.GetAddressOf(),
		cpStructuredSRV.GetAddressOf(),
		cpStructuredUAV.GetAddressOf()
	);
}

template<typename T>
inline StructuredBuffer<T>::~StructuredBuffer()
{
}

template<typename T>
inline void StructuredBuffer<T>::UpdateStructuredBuffer()
{
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		static_cast<std::vector<T>&>(*this), D3D11_MAP::D3D11_MAP_WRITE,
		cpStagingBuffer.Get()
	);

	DirectXDevice::pDeviceContext->CopyResource(
		cpStructuredBuffer.Get(), cpStagingBuffer.Get()
	);
}
