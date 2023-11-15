#pragma once


enum VSConstBufferType : unsigned int
{
	VS_CBUFF_MODELMAT,
	VS_CBUFF_VIEWPROJMAT
};

enum DSSRVType : unsigned int
{
	DS_SRV_HEIGHT,
};

enum DSConstBufferType : unsigned int
{
	DS_CBUFF_VIEWPROJMAT
};

enum PSConstBufferType : unsigned int
{
	PS_CBUFF_LIGHT,
	PS_CBUFF_MODELID,
	PS_CBUFF_TEXTUREFLAGS,
	PS_CBUFF_BLEND_CONST,
	PS_CBUFF_VIEWPORT
};


enum PSSRVType : unsigned int
{
	PS_SRV_AO,
	PS_SRV_COLOR,
	PS_SRV_METALNESS,
	PS_SRV_ROUGHNESS,
	PS_SRV_NORMAL,
	PS_SRV_MODEL_TEXTURE_NUM,
	PS_SRV_CUBEMAP_SPECULAR,
	PS_SRV_CUBEMAP_IRRADIANCE,
	PS_SRV_FILTER1,
	PS_SRV_FILTER2,
};

enum GSConstBufferType : unsigned int
{
	GS_CBUFF_TEXTUREFLAGS,
	GS_CBUFF_VIEWPROJMAT
};

enum GSSRVType : unsigned int
{
	GS_SRV_NORMAL
};