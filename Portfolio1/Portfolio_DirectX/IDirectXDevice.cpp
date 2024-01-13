#include "IDirectXDevice.h"

IDirectXDevice::IDirectXDevice(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContext)
{
}

IDirectXDevice::~IDirectXDevice()
{
}
