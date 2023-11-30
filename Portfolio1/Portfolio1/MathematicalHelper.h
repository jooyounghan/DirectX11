#pragma once
#include <directxmath/DirectXMath.h>
#include <algorithm>

class MathematicalHelper
{
public:
	static DirectX::XMVECTOR MakeReflect(const DirectX::XMVECTOR& normalVec, const DirectX::XMVECTOR& inVec);
};