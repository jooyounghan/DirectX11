#pragma once
#include "ICommand.h"

template <typename T>
class IncreaseCommand : public ICommand
{
public:
	IncreaseCommand(T* pTargetIn, const T& increaseFactor, const T& increaseRateIn);
	virtual ~IncreaseCommand();

public:
	virtual void Command(const float& fDelay) override;

protected:
	T* pTarget;
	const T increaseFactor;
	const T increaseRate;
};

template<typename T>
inline IncreaseCommand<T>::IncreaseCommand(T* pTargetIn, const T& increaseFactorIn, const T& increaseRateIn)
	: ICommand(), pTarget(pTargetIn), increaseFactor(increaseFactorIn), increaseRate(increaseRateIn)
{
}

template<typename T>
inline IncreaseCommand<T>::~IncreaseCommand()
{
}

template<typename T>
inline void IncreaseCommand<T>::Command(const float& fDelay)
{
	*pTarget += increaseRate * increaseRate * fDelay;
}
