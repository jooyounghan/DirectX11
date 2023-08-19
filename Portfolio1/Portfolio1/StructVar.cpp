#include "StructVar.h"
#include "DefineVar.h"

void ModelTransform::Init(OUT ModelTransform* pModelTransformation)
{
	AutoZeroMemory(*pModelTransformation);
	pModelTransformation->xmvScale = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
	pModelTransformation->sPositionAngle.fRoll = 0.f;
	pModelTransformation->sPositionAngle.fPitch = 0.f;
	pModelTransformation->sPositionAngle.fYaw = 0.f;
	pModelTransformation->xmvTranslation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

DirectX::XMMATRIX ModelTransform::GetAffineTransformMatrix(IN const ModelTransform& pModelTransformation)
{
	DirectX::XMMATRIX xmmScale = DirectX::XMMatrixScaling(
		pModelTransformation.xmvScale.m128_f32[0],
		pModelTransformation.xmvScale.m128_f32[1],
		pModelTransformation.xmvScale.m128_f32[2]
	);

	DirectX::XMMATRIX xmmRotation = DirectX::XMMatrixRotationRollPitchYaw(
		pModelTransformation.sPositionAngle.fPitch,
		pModelTransformation.sPositionAngle.fYaw,
		pModelTransformation.sPositionAngle.fRoll
	);

	DirectX::XMMATRIX xmmTranslation = DirectX::XMMatrixTranslation(
		pModelTransformation.xmvTranslation.m128_f32[0],
		pModelTransformation.xmvTranslation.m128_f32[1],
		pModelTransformation.xmvTranslation.m128_f32[2]
	);

	
	return DirectX::XMMatrixTranspose(xmmScale * xmmRotation * xmmTranslation);
}
