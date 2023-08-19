#pragma once
#include <directxmath/DirectXMath.h>
#include <algorithm>

class MathematicalHelper
{
public:
	static DirectX::XMVECTOR MakeQuaternion(const DirectX::XMVECTOR& RotOrigin, const float& fDegree);
};