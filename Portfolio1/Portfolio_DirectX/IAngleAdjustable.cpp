#include "IAngleAdjustable.h"
#include "DefineVar.h"

IAngleAdjustable::IAngleAdjustable(
	const float& fPitchRadIn,
	const float& fYawRadIn,
	const float& fRollRadIn
)
{
	AutoZeroMemory(sAngles);
	sAngles.fPitchRad = fPitchRadIn;
	sAngles.fYawRad = fYawRadIn;
	sAngles.fRollRad = fRollRadIn;
}

IAngleAdjustable::~IAngleAdjustable()
{
}
