#pragma once

enum VSConstBufferType : unsigned int
{
	VS_ModelMatrix,
	VS_ViewProjMatrix
};

enum DSSRVType : unsigned int
{
	DS_HEIGHT,
};

enum DSConstBufferType : unsigned int
{
	DS_ViewProjMatrix
};

enum PSConstBufferType : unsigned int
{
	PS_LightStructure,
	PS_ModelID,
	PS_TextureFlags
};

enum PSSRVType : unsigned int
{
	PS_AO,
	PS_COLOR,
	PS_METALNESS,
	PS_ROUGHNESS,
	PS_NORMAL
};

enum GSConstBufferType : unsigned int
{
	GS_TextureFlags,
	GS_ViewProjMatrix
};

enum GSSRVType : unsigned int
{
	GS_NORMAL
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