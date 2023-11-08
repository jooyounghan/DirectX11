#pragma once

enum CubeMapVSConstBufferType : unsigned int
{
	CUBEMAP_VS_VIEWPROJMAT
};

enum CubeMapPSConstBufferType : unsigned int
{
	CUBEMAP_PS_TEXTUREFLAGS
};

enum CubeMapPSSRVType : unsigned int
{
	CUBEMAP_PS_CUBEMAP
};
