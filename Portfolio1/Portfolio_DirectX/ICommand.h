#pragma once

enum KeyCode : unsigned __int64
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

class ICommand
{
public:
	ICommand() {};
	~ICommand() {};

public:
	virtual void Command(const float& fDelay) = 0;
};

