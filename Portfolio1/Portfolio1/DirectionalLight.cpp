#include "DirectionalLight.h"
#include "ID3D11Helper.h"
#include <directxmath/DirectXMath.h>

using namespace DirectX;

DirectionalLight::DirectionalLight(
	IN ComPtr<ID3D11Device>& cpDeviceIn,
	IN ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	IN const XMVECTOR& xmvLocationIn,
	IN const XMVECTOR& xmvLightColorIn,
	IN const XMVECTOR& xmvDirectionIn
)
	: ILight(cpDeviceIn, cpDeviceContextIn)
{
	AddLightSet();
	LightSet& sLightSet = vLightSets.back();
	AutoZeroMemory(sLightSet);
	sLightSet.eLightType = LightType::Directional;
	sLightSet.xmvLocation = xmvLocationIn;
	sLightSet.xmvLightColor = xmvLightColorIn;
	sLightSet.xmvDirection = xmvDirectionIn;
}

DirectionalLight::~DirectionalLight()
{
	RemoveLightSet();
}