#pragma once
#include <windows.h>
#include <directxmath/DirectXMath.h>

struct PositionAngle
{
	float fRoll;
	float fPitch;
	float fYaw;
};

class TransformProperties
{
public:
	TransformProperties();

public:
	DirectX::XMVECTOR	xmvScale;
	PositionAngle		sPositionAngle;
	DirectX::XMVECTOR	xmvTranslation;

public:
	static DirectX::XMMATRIX GetAffineTransformMatrix(IN const TransformProperties& pModelTransformation);
};

class TransformationBufferData
{
public:
	TransformationBufferData();

public:
	DirectX::XMMATRIX xmmTransformedMat;
	DirectX::XMMATRIX xmmInvTransformedMat;

public:
	static TransformationBufferData CreateTransfomredMatrix(IN const DirectX::XMMATRIX& xmmTransformedMatIn);
	static void CreateTransfomredMatrix(IN const DirectX::XMMATRIX& xmmTransformedMatIn, OUT TransformationBufferData& transformedMatrixOut);
};