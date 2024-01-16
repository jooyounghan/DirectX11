#include "IScalable.h"
#include "DefineVar.h"

IScalable::IScalable()
{
	AutoZeroMemory(sScales);
	sScales.fScaleX = 1.f;
	sScales.fScaleY = 1.f;
	sScales.fScaleZ = 1.f;
}

IScalable::~IScalable()
{
}
