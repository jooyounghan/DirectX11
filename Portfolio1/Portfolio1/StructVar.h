#pragma once
#include <windows.h>
#include <directxmath/DirectXMath.h>

struct Vertex
{
	float x, y, z, w;
};

struct PositionAngle
{
	float fRoll;
	float fPitch;
	float fYaw;
};

struct ModelTransform
{
	static void Init(OUT ModelTransform* pModelTransformation);
	static DirectX::XMMATRIX GetAffineTransformMatrix(IN const ModelTransform& pModelTransformation);

	DirectX::XMVECTOR	xmvScale;
	PositionAngle		sPositionAngle;
	DirectX::XMVECTOR	xmvTranslation;
};