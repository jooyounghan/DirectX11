#include "MathematicalHelper.h"

DirectX::XMVECTOR MathematicalHelper::MakeQuaternion(const DirectX::XMVECTOR& RotOrigin, const float& fDegree)
{
	const float& fSinRad = sinf(DirectX::XMConvertToRadians(fDegree));
	const float& fCosRad = cosf(DirectX::XMConvertToRadians(fDegree));
	return DirectX::XMVectorSet(RotOrigin.m128_f32[0] * fSinRad, RotOrigin.m128_f32[1] * fSinRad, RotOrigin.m128_f32[2] * fSinRad, fCosRad);
}
