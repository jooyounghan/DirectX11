#include "DrawerInterface.h"
#include "ModelInterface.h"
#include "CameraAdapter.h"

DrawerInterface::DrawerInterface(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
}

DrawerInterface::~DrawerInterface()
{
}
