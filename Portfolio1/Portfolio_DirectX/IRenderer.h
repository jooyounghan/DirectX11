#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

class IRenderer
{
public:
	IRenderer();
	~IRenderer();

public:
	virtual void SetMessageFilter() = 0;
	virtual void ResetMessageFilter() = 0;
};
