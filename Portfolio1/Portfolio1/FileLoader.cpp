#include "FileLoader.h"
#include "StructVar.h"

#define __STDC_LIB_EXT1__
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

extern void Console(const char* text);

ImageContainer ImageContainer::ExampleTextureAO;
ImageContainer ImageContainer::ExampleTextureDiffuse;
ImageContainer ImageContainer::ExampleTextureHeight;
ImageContainer ImageContainer::ExampleTextureNormal;
ImageContainer ImageContainer::ExampleTextureReflection;

void FileLoader::LoadImageFromFile(IN const std::string& sfileName, OUT ImageContainer* pImageVar)
{
    int iWidth, iHeight, iChannel;
    pImageVar->pData = stbi_load(sfileName.c_str(), &iWidth, &iHeight, &iChannel, 0);
    pImageVar->uiWidth = (UINT)iWidth;
    pImageVar->uiHeight = (UINT)iHeight;
    pImageVar->uiChannel = (UINT)iChannel;

    if (pImageVar->pData == nullptr)
    {
        Console((sfileName + "를 불러오는데 실패하였습니다.").c_str());
        return;
    }
}

void FileLoader::PreLoadFiles()
{
    LoadImageFromFile(".\\Texture\\GreyPorousRock\\grey_porous_rock_40_56_ao.jpg", &ImageContainer::ExampleTextureAO);
    LoadImageFromFile(".\\Texture\\GreyPorousRock\\grey_porous_rock_40_56_diffuse.jpg", &ImageContainer::ExampleTextureDiffuse);
    LoadImageFromFile(".\\Texture\\GreyPorousRock\\grey_porous_rock_40_56_height.jpg", &ImageContainer::ExampleTextureHeight);
    LoadImageFromFile(".\\Texture\\GreyPorousRock\\grey_porous_rock_40_56_normal.jpg", &ImageContainer::ExampleTextureNormal);
    LoadImageFromFile(".\\Texture\\GreyPorousRock\\grey_porous_rock_40_56_reflection.jpg", &ImageContainer::ExampleTextureReflection);
}

ImageContainer::ImageContainer()
    : uiWidth(0), uiHeight(0), uiChannel(0), pData(nullptr)
{}

ImageContainer::~ImageContainer()
{
    if (pData != nullptr)
    {
        //stbi_image_free(this->ucData);
        this->pData = nullptr;
    }
}

void ImageContainer::ExtendChannel(unsigned int uiChannelIn)
{
    if (uiChannel < uiChannelIn)
    {
        const unsigned int& uiTotalDataSizeAfter = uiWidth * uiHeight * uiChannelIn;
        uint8_t* pDataNew = new uint8_t[uiTotalDataSizeAfter];
        memset(pDataNew, 255, uiTotalDataSizeAfter);

        for (unsigned int h = 0; h < uiHeight; ++h)
        {
            for (unsigned int w = 0; w < uiWidth; ++w)
            {
                memcpy(pDataNew + (h * uiWidth + w) * uiChannelIn, pData + (h * uiWidth + w) * uiChannel, uiChannel);
            }
        }

        STBI_FREE(pData);
        pData = pDataNew;
        uiChannel = uiChannelIn;
    }
}
