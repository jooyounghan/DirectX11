#pragma once

#include <vector>
#include <functional>
#include <utility>

using std::vector;
using std::function;

template<typename RetType, typename... Params>
class Delegater
{
	template<typename ClassType>
	using MemberCbkFunc = RetType(ClassType::*)(Params...);
	
	using CbkFunc = function<RetType(Params...)>;

public:
	~Delegater();
	void Notify(Params... params);

protected:
	vector<CbkFunc> m_delegateds_;

public:
	template<typename ClassType>
	void Add(ClassType* instance, MemberCbkFunc<ClassType> member_function);
	void Add(CbkFunc callback_function);

};

template<typename RetType, typename ...Params>
void Delegater<RetType, Params...>::Notify(Params... params)
{
	for (auto& delegated : m_delegateds_)
	{
		delegated(std::forward<Params>(params)...);
	}
}

template<typename RetType, typename ...Params>
inline void Delegater<RetType, Params...>::Add(CbkFunc callback_function)
{
	m_delegateds_.push_back(callback_function);
}

template<typename RetType, typename ...Params>
template<typename ClassType>
inline void Delegater<RetType, Params...>::Add(ClassType* instance, MemberCbkFunc<ClassType> member_function)
{
	auto wrapped_function = [=](Params&&... params)
	{
		instance->*member_function(std::forward<Params>(params)...);
	};
	Add(wrapped_function);
}
