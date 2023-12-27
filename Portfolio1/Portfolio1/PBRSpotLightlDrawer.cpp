#include "PBRSpotLightDrawer.h"
#include "ID3D11Helper.h"

using namespace Microsoft::WRL;

PBRSpotLightDrawer::PBRSpotLightDrawer(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: APBRDirectLightDrawer(pDeviceIn, pDeviceContextIn)
{
	ID3D11Helper::CreatePS(pDeviceIn, L"BaseModelWithSpotLight.hlsl", cpBasePixelShader.GetAddressOf());
}

PBRSpotLightDrawer::~PBRSpotLightDrawer()
{
}
