#include "IRectangle.h"

IRectangle::IRectangle()
	: uiWidth(0), uiHeight(0)
{
}

IRectangle::IRectangle(
	const UINT& uiWidthIn, 
	const UINT& uiHeightIn
)
	: uiWidth(uiWidthIn), uiHeight(uiHeightIn)
{
}

IRectangle::~IRectangle()
{
}