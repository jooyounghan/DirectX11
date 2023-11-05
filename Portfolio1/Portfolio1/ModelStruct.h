#pragma once

struct PositionVector
{
	float x;
	float y;
	float z;
	float w = 1.f;
};

struct Vector
{
	float x;
	float y;
	float z;
	float w = 0.f;
};

struct TextureCoord
{
	float x;
	float y;
};

struct Vertex
{
	PositionVector	sPosVec;
	TextureCoord	sTexCoord;
	Vector			sNorVec;
	Vector			sTangVec;
};

enum MODEL_TEXTURE
{
	MODEL_TEXTURE_AO,
	MODEL_TEXTURE_COLOR,
	MODEL_TEXTURE_METALNESS,
	MODEL_TEXTURE_ROUGHNESS,
	MODEL_TEXTURE_NORMAL,
	MODEL_TEXTURE_HEIGHT,
	MODEL_TEXTURE_NUM
};
