#include "ATransformerable.h"
#include "DefineVar.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"

using namespace DirectX;

ATransformerable::ATransformerable()
	: IMovable(), IScalable(), IAngleAdjustable()
{
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, 
		sTransformation, D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, 
		NULL, cpTransformationBuffer.GetAddressOf()
	);
}

ATransformerable::~ATransformerable()
{
}

void ATransformerable::UpdateTranformMat()
{
	sTransformation.xmmTransformation = MathematicalHelper::MakeAffineTransformation(
		sScales.fScaleX, sScales.fScaleY, sScales.fScaleZ,
		sAngles.fPitch, sAngles.fYaw, sAngles.fRoll,
		xmvPosition.m128_f32[0], xmvPosition.m128_f32[1], xmvPosition.m128_f32[2]
	);

	sTransformation.xmmInvTransformation = XMMatrixInverse(nullptr, sTransformation.xmmTransformation);
	sTransformation.xmmTransformation = XMMatrixTranspose(sTransformation.xmmTransformation);
}

