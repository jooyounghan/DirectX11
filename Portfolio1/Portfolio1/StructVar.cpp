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

	
	return XMMatrixTranspose(xmmScale * xmmRotation * xmmTranslation);
}

TransformedMatrix TransformedMatrix::CreateTransfomredMatrix(const XMMATRIX& xmmInvTransformedMatIn)
{
	TransformedMatrix result;
	result.xmmTransformedMat = xmmInvTransformedMatIn;

	result.xmmInvTransformedMat = XMMatrixTranspose(xmmInvTransformedMatIn);
	result.xmmInvTransformedMat = XMMatrixInverse(nullptr, result.xmmInvTransformedMat);
	// Transpose 2번의 경우 상쇄되므로 생략
	return result;
}
