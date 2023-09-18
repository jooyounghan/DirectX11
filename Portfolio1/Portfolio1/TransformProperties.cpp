#include "TransformProperties.h"
#include "DefineVar.h"
using namespace DirectX;

TransformProperties::TransformProperties()
{
	xmvScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
	AutoZeroMemory(sPositionAngle);
	xmvTranslation = XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

DirectX::XMMATRIX TransformProperties::GetAffineTransformMatrix(IN const TransformProperties& pModelTransformation)
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

TransformationBufferData::TransformationBufferData()
	: xmmTransformedMat(XMMatrixIdentity()), xmmInvTransformedMat(XMMatrixIdentity())
{

}

TransformationBufferData TransformationBufferData::CreateTransfomredMatrix(IN const DirectX::XMMATRIX& xmmTransformedMatIn)
{
	TransformationBufferData result;
	CreateTransfomredMatrix(xmmTransformedMatIn, result);
	return result;
}

void TransformationBufferData::CreateTransfomredMatrix(IN const DirectX::XMMATRIX& xmmTransformedMatIn, OUT TransformationBufferData& transformedMatrixOut)
{
	transformedMatrixOut.xmmTransformedMat = XMMatrixTranspose(xmmTransformedMatIn);
	transformedMatrixOut.xmmInvTransformedMat = XMMatrixInverse(nullptr, xmmTransformedMatIn);
	// Transpose 2번의 경우 상쇄되므로 생략
}
