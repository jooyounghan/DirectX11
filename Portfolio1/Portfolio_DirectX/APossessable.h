#pragma once

enum EKeyCode : unsigned __int64
{
	Zero = 0x30,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	A = 0x41,
	B, C, D, E, F,
	G, H, I, J, K,
	L, M, N, O, P,
	Q, R, S, T, U,
	V, W, X, Y, Z,
	NUM_KEY
};

class APossessable
{
public:
	APossessable();
	~APossessable();

protected:
	bool pPressed[EKeyCode::NUM_KEY];

public:
	inline void Press(const EKeyCode& eKeyCode) { pPressed[eKeyCode] = true; }
	inline void Release(const EKeyCode& eKeyCode) { pPressed[eKeyCode] = false; }
	inline void Toggle(const EKeyCode& eKeyCode) { pPressed[eKeyCode] = !pPressed[eKeyCode]; }

public:
	virtual void ManageKeyBoardInput(const float& fDelay) = 0;
	virtual void ManageMouseInput(const int& iMouseXIn, const int& iMouseYIn) = 0;
};

