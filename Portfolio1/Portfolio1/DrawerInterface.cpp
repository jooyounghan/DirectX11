#include "DrawerInterface.h"
#include "ModelInterface.h"
#include "CameraInterface.h"

DrawerInterface::DrawerInterface(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), pModel(nullptr), pCamera(nullptr), pLightManager(nullptr)
{
}

DrawerInterface::~DrawerInterface()
{
}
