#include "MathematicalHelper.h"

using namespace DirectX;

XMVECTOR MathematicalHelper::MakeReflect(const XMVECTOR& normalVec, const XMVECTOR& inVec)
{
	XMVECTOR dotProduct = XMVector3Dot(normalVec, XMVectorNegate(inVec));
	return inVec + 2 * normalVec * (dotProduct.m128_f32[0]);
}
