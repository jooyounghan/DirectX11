#pragma once

enum SamplerType : unsigned int
{
	WRAP_SAMPLER,
	CLAMP_SAMPLER
};

enum VSConstBufferType : unsigned int
{
	VS_CBUFF_MODELMAT,
	VS_CBUFF_CAMERA_INFO,
	VS_CBUFF_LIGHTBASE,
	VS_CBUFF_LIGHT_VIEWPROJ,
};

enum DSSRVType : unsigned int
{
	DS_SRV_HEIGHT,
};

enum DSConstBufferType : unsigned int
{
	DS_CBUFF_CAMERA_INFO
};

enum PSConstBufferType : unsigned int
{
	PS_CBUFF_LIGHTBASE,
	PS_CBUFF_MODELID,
	PS_CBUFF_MODEL_TEXTURE_FLAGS,
	PS_CBUFF_MODEL_CONST,
	PS_CBUFF_CUBE_TEXTURE_FLAGS,
	PS_CBUFF_BLEND_CONST,
	PS_CBUFF_CAMERA_INFO,
	PS_CBUFF_VIEWPORT
};


enum PSSRVType : unsigned int
{
	PS_SRV_AO,
	PS_SRV_COLOR,
	PS_SRV_METALNESS,
	PS_SRV_ROUGHNESS,
	PS_SRV_EMISSION,
	PS_SRV_NORMAL,
	PS_SRV_CUBEMAP_SPECULAR,
	PS_SRV_CUBEMAP_IRRADIANCE,
	PS_SRV_CUBEMAP_BRDF,
	PS_SRV_FILTER1,
	PS_SRV_FILTER2,
	PS_SRV_MIRROR_SELF,
	PS_SRV_DEPTH_ONLY_OR_X,
	PS_SRV_NEG_X,
	PS_SRV_Y,
	PS_SRV_NEG_Y,
	PS_SRV_Z,
	PS_SRV_NEG_Z,
};

enum GSConstBufferType : unsigned int
{
	GS_CBUFF_TEXTUREFLAGS,
	GS_CBUFF_CAMERA_INFO
};

enum GSSRVType : unsigned int
{
	GS_SRV_NORMAL
};

enum HSConstantBufferType : unsigned int
{
	HS_CBUFF_CAMERA_INFO
};
