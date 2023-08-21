#pragma once
#include <windows.h>
#include <directxmath/DirectXMath.h>

struct PositionVector
{
	float x;
	float y;
	float z;
	float w = 1.f;
};

struct TextureCoord
{
	float x, y;
};

struct NormalVector
{
	float x;
	float y;
	float z;
	float w = 0.f;
};

struct Vertex
{
	PositionVector	sPosVec;
	TextureCoord	sTexCoord;
	NormalVector	sNorVec;
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