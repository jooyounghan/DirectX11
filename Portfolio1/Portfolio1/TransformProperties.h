#pragma once
#include <windows.h>
#include <directxmath/DirectXMath.h>

class TransformProperties
{
public:
	TransformProperties();

public:
	DirectX::XMVECTOR	xmvScale;
	struct {
		float fRoll;
		float fPitch;
		float fYaw;
	}					sPositionAngle;
	DirectX::XMVECTOR	xmvTranslation;

public:
	static DirectX::XMMATRIX GetAffineTransformMatrix(IN TransformProperties* pModelTransformation);
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
	static void CreateTransfomredMatrix(IN const DirectX::XMMATRIX& xmmTransformedMatIn, OUT TransformationBufferData* pTransformationBufferDataIn);
};