#include "LightInterface.h"
#include "ID3D11Helper.h"
#include "DefineVar.h"

LightInterface::LightInterface(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn)
{
}

LightInterface::~LightInterface()
{
}