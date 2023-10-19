#pragma once
#include "FilterInterface.h"

template<typename FilterImpl>
class FilterAdapter : public FilterInterface
{
public:
	template<typename ...Args>
	FilterAdapter(Args... arg);
	~FilterAdapter();

public:
	FilterImpl filterImpl;
};

template<typename FilterImpl>
template<typename ...Args>
FilterAdapter<FilterImpl>::FilterAdapter(Args ...arg)
{
	
}

template<typename FilterImpl>
FilterAdapter<FilterImpl>::~FilterAdapter()
{
}
