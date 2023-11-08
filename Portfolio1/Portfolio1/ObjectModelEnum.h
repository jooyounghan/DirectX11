#pragma once

enum ObjectVSConstBufferType : unsigned int
{
	OBJECT_VS_MODELMAT,
	OBJECT_VS_VIEWPROJMAT
};

enum ObjectDSSRVType : unsigned int
{
	OBJECT_DS_HEIGHT,
};

enum ObjectDSConstBufferType : unsigned int
{
	OBJECT_DS_VIEWPROJMAT
};

enum ObjectPSConstBufferType : unsigned int
{
	OBJECT_PS_LIGHT,
	OBJECT_PS_MODELID,
	OBJECT_PS_TEXTUREFLAGS
};


enum ObjectPSSRVType : unsigned int
{
	OBJECT_PS_AO,
	OBJECT_PS_COLOR,
	OBJECT_PS_METALNESS,
	OBJECT_PS_ROUGHNESS,
	OBJECT_PS_NORMAL
};

enum ObjectGSConstBufferType : unsigned int
{
	OBJECT_GS_TEXTUREFLAGS,
	OBJECT_GS_VIEWPROJMAT
};

enum ObjectGSSRVType : unsigned int
{
	OBJECT_GS_NORMAL
};