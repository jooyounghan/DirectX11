#include "DrawerInterface.h"
#include "ModelInterface.h"
#include "CameraInterface.h"
#include "LightManager.h"
#include "ID3D11Helper.h"

DrawerInterface::DrawerInterface(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn)
{
}

DrawerInterface::~DrawerInterface()
{
}