#include "MathematicalHelper.h"

DirectX::XMVECTOR MathematicalHelper::MakeReflect(const DirectX::XMVECTOR& normalVec, const DirectX::XMVECTOR& inVec)
{
	DirectX::XMVECTOR dotProduct = DirectX::XMVector3Dot(normalVec, DirectX::XMVectorNegate(inVec));
	return DirectX::XMVectorAdd(inVec, DirectX::XMVectorScale(normalVec, 2.f * dotProduct.m128_f32[0]));
}

DirectX::XMMATRIX MathematicalHelper::MakePerspectiveViewProjMatrix(
	const DirectX::XMVECTOR& xmvCameraPosition, 
	const DirectX::XMVECTOR& xmvCameraDirection, 
	const DirectX::XMVECTOR& xmvCameraUp, 
	const float& fFovRadian, const float& fAspectRatio, 
	const float& fNearZ, const float& fFarZ
)
{
	return DirectX::XMMatrixLookToLH(xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		DirectX::XMMatrixPerspectiveFovLH(fFovRadian, fAspectRatio, fNearZ, fFarZ);
}

DirectX::XMMATRIX MathematicalHelper::MakePerspectiveViewProjMatrix(
	const DirectX::XMVECTOR& xmvCameraPosition,
	DirectX::XMVECTOR&& xmvCameraDirection,
	DirectX::XMVECTOR&& xmvCameraUp,
	const float& fFovRadian, const float& fAspectRatio,
	const float& fNearZ, const float& fFarZ
)
{
	return DirectX::XMMatrixLookToLH(xmvCameraPosition, xmvCameraDirection, xmvCameraUp) *
		DirectX::XMMatrixPerspectiveFovLH(fFovRadian, fAspectRatio, fNearZ, fFarZ);
}

DirectX::XMMATRIX MathematicalHelper::MakeAffineTransformation(
	const float& fXScale, const float& fYScale, const float& fZScale, 
	const float& fPitchDeg, const float& fYawDeg, const float& fRollDeg, 
	const float& fXTranslation, const float& fYTranslation, const float& fZTranslation
) 
{
	DirectX::XMMATRIX xmmScale = DirectX::XMMatrixScaling(fXScale, fYScale, fZScale);
	DirectX::XMMATRIX xmmRotation = DirectX::XMMatrixRotationRollPitchYaw(fPitchDeg, fYawDeg, fRollDeg);
	DirectX::XMMATRIX xmmTranslation = DirectX::XMMatrixTranslation(fXTranslation, fYTranslation, fZTranslation);

	return xmmScale * xmmRotation * xmmTranslation;
}
