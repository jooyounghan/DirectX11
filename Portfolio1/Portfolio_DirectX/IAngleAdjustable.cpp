#include "IAngleAdjustable.h"
#include "DefineVar.h"

IAngleAdjustable::IAngleAdjustable(
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn
)
{
	AutoZeroMemory(sAnglesDegree);
	sAnglesDegree.fPitchDeg = fPitchDegIn;
	sAnglesDegree.fYawDeg = fYawDegIn;
	sAnglesDegree.fRollDeg = fRollDegIn;
}

IAngleAdjustable::~IAngleAdjustable()
{
}

DirectX::XMMATRIX IAngleAdjustable::GetTransformedMatrix()
{
	return DirectX::XMMatrixRotationRollPitchYaw(
		DirectX::XMConvertToRadians(sAnglesDegree.fPitchDeg),
		DirectX::XMConvertToRadians(sAnglesDegree.fYawDeg),
		DirectX::XMConvertToRadians(sAnglesDegree.fRollDeg)
	);
}
