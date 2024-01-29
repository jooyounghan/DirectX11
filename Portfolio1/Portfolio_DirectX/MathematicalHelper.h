#pragma once
#include <directxmath/DirectXMath.h>
#include <algorithm>

class MathematicalHelper
{
public:
	static DirectX::XMVECTOR MakeReflect(const DirectX::XMVECTOR& normalVec, const DirectX::XMVECTOR& inVec);

public:
	static DirectX::XMMATRIX MakeViewProjMatrix(
		const DirectX::XMVECTOR& xmvCameraPosition,
		const DirectX::XMVECTOR& xmvCameraDirection,
		const DirectX::XMVECTOR& xmvCameraUp,
		const float& fFovAngle,
		const float& fAspectRatio,
		const float& fNearZ,
		const float& fFarZ
	);

	static DirectX::XMMATRIX MakeViewProjMatrix(
		const DirectX::XMVECTOR& xmvCameraPosition,
		DirectX::XMVECTOR&& xmvCameraDirection,
		DirectX::XMVECTOR&& xmvCameraUp,
		const float& fFovAngle,
		const float& fAspectRatio,
		const float& fNearZ,
		const float& fFarZ
	);

public:
	static DirectX::XMMATRIX MakeAffineTransformation(
		const float& fXScale, const float& fYScale, const float& fZScale,
		const float& fPitchDeg, const float& fYawDeg, const float& fRollDeg,
		const float& fXTranslation, const float& fYTranslation, const float& fZTranslation
	);
};