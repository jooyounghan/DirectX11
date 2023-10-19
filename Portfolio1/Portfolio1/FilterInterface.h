#pragma once
#include <functional>

class FilterInterface
{
public:
	FilterInterface();
	virtual ~FilterInterface();

public:
	std::function<void()> StartFilter;
};

