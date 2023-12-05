#include "DrawerInterface.h"
#include "ModelInterface.h"
#include "CameraInterface.h"
#include "LightManager.h"
#include "ID3D11Helper.h"

DrawerInterface::DrawerInterface(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, NULL, cpDevice.Get(), cpDrawerWrapSampler.GetAddressOf());
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, NULL, cpDevice.Get(), cpDrawerClampSampler.GetAddressOf());
}

DrawerInterface::~DrawerInterface()
{
}