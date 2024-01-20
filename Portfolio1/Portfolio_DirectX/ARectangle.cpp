#include "ARectangle.h"

ARectangle::ARectangle()
	: uiWidth(0), uiHeight(0)
{
}

ARectangle::ARectangle(
	const UINT& uiWidthIn, 
	const UINT& uiHeightIn
)
	: uiWidth(uiWidthIn), uiHeight(uiHeightIn)
{
}

ARectangle::~ARectangle()
{
}

void ARectangle::SetRectangle(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	uiWidth = uiWidthIn;
	uiHeight = uiHeightIn;
}
