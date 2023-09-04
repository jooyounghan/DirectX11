#include "StructVar.h"
#include "DefineVar.h"

using namespace DirectX;

void ModelTransform::Init(OUT ModelTransform* pModelTransformation)
{
	AutoZeroMemory(*pModelTransformation);
	pModelTransformation->xmvScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
	pModelTransformation->sPositionAngle.fRoll = 0.f;
	pModelTransformation->sPositionAngle.fPitch = 0.f;
	pModelTransformation->sPositionAngle.fYaw = 0.f;
	pModelTransformation->xmvTranslation = XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

XMMATRIX ModelTransform::GetAffineTransformMatrix(IN const ModelTransform& pModelTransformation)
{
	XMMATRIX xmmScale = XMMatrixScaling(
		pModelTransformation.xmvScale.m128_f32[0],
		pModelTransformation.xmvScale.m128_f32[1],
		pModelTransformation.xmvScale.m128_f32[2]
	);

	XMMATRIX xmmRotation = XMMatrixRotationRollPitchYaw(
		pModelTransformation.sPositionAngle.fPitch,
		pModelTransformation.sPositionAngle.fYaw,
		pModelTransformation.sPositionAngle.fRoll
	);

	XMMATRIX xmmTranslation = XMMatrixTranslation(
		pModelTransformation.xmvTranslation.m128_f32[0],
		pModelTransformation.xmvTranslation.m128_f32[1],
		pModelTransformation.xmvTranslation.m128_f32[2]
	);

	
	return xmmScale * xmmRotation * xmmTranslation;
}

TransformedMatrix TransformedMatrix::CreateTransfomredMatrix(const XMMATRIX& xmmTransformedMatIn)
{
	TransformedMatrix result;
	result.xmmTransformedMat = XMMatrixTranspose(xmmTransformedMatIn);
	result.xmmInvTransformedMat = XMMatrixInverse(nullptr, xmmTransformedMatIn);
	// Transpose 2번의 경우 상쇄되므로 생략
	return result;
}
