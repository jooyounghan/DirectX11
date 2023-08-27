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
	float x;
	float y;
	float dummy1 = 0;
	float dummy2 = 0;
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

struct TransformedMatrix
{
	static TransformedMatrix CreateTransfomredMatrix(const DirectX::XMMATRIX& xmmInvTransformedMatIn);

	DirectX::XMMATRIX xmmTransformedMat;
	DirectX::XMMATRIX xmmInvTransformedMat;
};