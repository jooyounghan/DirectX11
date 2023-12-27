#include "PBRPointLightDrawer.h"
#include "ID3D11Helper.h"

using namespace Microsoft::WRL;

PBRPointLightDrawer::PBRPointLightDrawer(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn
)
	: APBRDirectLightDrawer(pDeviceIn, pDeviceContextIn)
{
	ID3D11Helper::CreatePS(pDeviceIn, L"BaseModelWithPointLight.hlsl", cpBasePixelShader.GetAddressOf());
}

PBRPointLightDrawer::~PBRPointLightDrawer()
{
}
