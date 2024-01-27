#pragma once
#include <memory>

#include "ICommand.h"

class APossessable
{
public:
	APossessable();
	~APossessable();

protected:
	class ICommand* pCommand[EKeyCode::NUM_KEY];
	bool			pPressed[EKeyCode::NUM_KEY];

public:
	inline void Press(const EKeyCode& eKeyCode, const float& fDelta) { if (pCommand[eKeyCode]) pCommand[eKeyCode]->Command(fDelta); }
	inline void Toggle(const EKeyCode& eKeyCode) { pPressed[eKeyCode] = !pPressed[eKeyCode]; }

public:
	virtual void SetDefaultCommand() = 0;

	virtual void ManageKeyBoardInput(const float& fDelay) = 0;
	virtual void ManageMouseInput(const float& fDelay) = 0;
};

