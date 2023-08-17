#pragma once
#include <directxmath/DirectXMath.h>



class MathematicalHelper
{
public:
	static DirectX::XMVECTOR MakeQuaternion(const DirectX::XMVECTOR& RotOrigin, const float& fDegree);
};