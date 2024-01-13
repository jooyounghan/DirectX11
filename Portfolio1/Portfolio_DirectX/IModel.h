#pragma once
#include <string>

class IModel
{
public:
	IModel();
	~IModel();

public:
	static uint32_t uiGlobalModelID;
	uint32_t uiModelID;

public:
	virtual void Load(const std::string& path) = 0;
	virtual void Render() = 0;
};

