#pragma once
#include <vector>
#include <string>
#include <Windows.h>

class ImageContainer
{
public:
	ImageContainer();
	~ImageContainer();

public:
	unsigned int	uiWidth;
	unsigned int	uiHeight;
	unsigned int	uiChannel;
	uint8_t*		pData;

public:
	void ExtendChannel(unsigned int uiChannelIn);

public:
	static ImageContainer			ExampleTextureAO;
	static ImageContainer			ExampleTextureDiffuse;
	static ImageContainer			ExampleTextureHeight;
	static ImageContainer			ExampleTextureNormal;
	static ImageContainer			ExampleTextureReflection;
};

class FileLoader
{
public:
	static void LoadImageFromFile(
		IN const std::string&		sfileName,
		OUT ImageContainer*			pImageContainer
	);

public:
	static void PreLoadFiles();
};

