#pragma once
#include <memory>

#include "ICommand.h"

class APossessable
{
public:
	APossessable();
	~APossessable();

protected:
	class ICommand* pCommand[KeyCode::NUM_KEY];
	bool			pPressed[KeyCode::NUM_KEY];

public:
	inline void Press(const KeyCode& eKeyCode, const float& fDelta) { if (pCommand[eKeyCode]) pCommand[eKeyCode]->Command(fDelta); }
	inline void Toggle(const KeyCode& eKeyCode) { pPressed[eKeyCode] = !pPressed[eKeyCode]; }

public:
	virtual void SetDefaultCommand() = 0;

	virtual void ManageKeyBoardInput(const float& fDelay) = 0;
	virtual void ManageMouseInput(const float& fDelay) = 0;
};

