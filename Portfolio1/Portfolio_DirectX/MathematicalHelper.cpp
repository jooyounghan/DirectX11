#include "MathematicalHelper.h"

using namespace DirectX;

XMVECTOR MathematicalHelper::MakeReflect(const XMVECTOR& normalVec, const XMVECTOR& inVec)
{
	XMVECTOR dotProduct = XMVector3Dot(normalVec, XMVectorNegate(inVec));
	return inVec + 2 * normalVec * (dotProduct.m128_f32[0]);
}

DirectX::XMMATRIX MathematicalHelper::MakeViewProjMatrix(
	const DirectX::XMVECTOR& xmvCameraPosition, 
	const DirectX::XMVECTOR& xmvCameraDirection, 
	const DirectX::XMVECTOR& xmvCameraUp, 
	const float& fFovRadian, const float& fAspectRatio, 
	const float& fNearZ, const float& fFarZ
)
{
	return XMMatrixLookToLH(xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		XMMatrixPerspectiveFovLH(fFovRadian, fAspectRatio, fNearZ, fFarZ);
}

DirectX::XMMATRIX MathematicalHelper::MakeViewProjMatrix(
	const DirectX::XMVECTOR& xmvCameraPosition,
	DirectX::XMVECTOR&& xmvCameraDirection,
	DirectX::XMVECTOR&& xmvCameraUp,
	const float& fFovRadian, const float& fAspectRatio,
	const float& fNearZ, const float& fFarZ
)
{
	return XMMatrixLookToLH(xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		XMMatrixPerspectiveFovLH(fFovRadian, fAspectRatio, fNearZ, fFarZ);
}


DirectX::XMMATRIX MathematicalHelper::MakeAffineTransformation(
	const float& fXScale, const float& fYScale, const float& fZScale, 
	const float& fPitchDeg, const float& fYawDeg, const float& fRollDeg, 
	const float& fXTranslation, const float& fYTranslation, const float& fZTranslation
) 
{
	XMMATRIX xmmScale = XMMatrixScaling(fXScale, fYScale, fZScale);
	XMMATRIX xmmRotation = XMMatrixRotationRollPitchYaw(fPitchDeg, fYawDeg, fRollDeg);
	XMMATRIX xmmTranslation = XMMatrixTranslation(fXTranslation, fYTranslation, fZTranslation);

	return xmmScale * xmmRotation * xmmTranslation;
}
