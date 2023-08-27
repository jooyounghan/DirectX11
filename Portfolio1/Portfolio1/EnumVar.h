#pragma once

enum VSConstBufferType : unsigned int
{
	ModelMatrix,
	ViewProjMatrix
};

enum VSSRVType : unsigned int
{
	VS_HEIGHT,
	VS_NORMAL
};

enum PSSRVType : unsigned int
{
	PS_AO,
	PS_DIFFUSE,
	PS_NORMAL,
	PS_REFLECT,
};

enum KeyCode
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
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	NUM_KEY
};

enum MoveDir
{
	Forward,
	Left,
	Backward,
	Right,
	NUM_DIR
};