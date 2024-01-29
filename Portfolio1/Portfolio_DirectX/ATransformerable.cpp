#include "ATransformerable.h"
#include "DefineVar.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"

using namespace DirectX;

ATransformerable::ATransformerable()
	: IMovable(0.f, 0.f, 0.f), IScalable(), IAngleAdjustable(0.f, 0.f, 0.f)
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

void ATransformerable::UpdateTranformationMatrix()
{
	sTransformation.xmmTransformation = MathematicalHelper::MakeAffineTransformation(
		sScales.fScaleX, sScales.fScaleY, sScales.fScaleZ,
		DirectX::XMConvertToRadians(sAnglesDegree.fPitchDeg),
		DirectX::XMConvertToRadians(sAnglesDegree.fYawDeg), 
		DirectX::XMConvertToRadians(sAnglesDegree.fRollDeg),
		xmvPosition.m128_f32[0], xmvPosition.m128_f32[1], xmvPosition.m128_f32[2]
	);

	sTransformation.xmmInvTransformation = XMMatrixInverse(nullptr, sTransformation.xmmTransformation);
	sTransformation.xmmTransformation = XMMatrixTranspose(sTransformation.xmmTransformation);
}

