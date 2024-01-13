#include "IModel.h"

uint32_t IModel::uiGlobalModelID = 0;

IModel::IModel() 
	: uiModelID(uiGlobalModelID++)
{

}

IModel::~IModel() {}
