#include "FileLoader.h"
#define __STDC_LIB_EXT1__
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

uint8_t* FileLoader::stbiw_load(wchar_t const* wFilename, int* x, int* y, int* comp, int req_comp)
{
    FILE* f;
    _wfopen_s(&f, wFilename, L"rb");
    uint8_t* result;
    if (!f) return stbi__errpuc("can't fopen", "Unable to open file");
    result = stbi_load_from_file(f, x, y, comp, req_comp);
    fclose(f);
    return result;
}


void FileLoader::stbiw_free(uint8_t* pRawData)
{
    return STBI_FREE(pRawData);
}

void FileLoader::ExtendChannel(uint8_t*& ucRawData, const int& iWidth, const int& iHeight, int& iChannelCurrent, const int& iChannelIn)
{
    if (iChannelCurrent < iChannelIn)
    {
        const int& iTotalDataSizeAfter = iWidth * iHeight * iChannelIn;
        uint8_t* pDataNew = new uint8_t[iTotalDataSizeAfter];
        memset(pDataNew, 255, iTotalDataSizeAfter);

        for (int h = 0; h < iHeight; ++h)
        {
            for (int w = 0; w < iWidth; ++w)
            {
                memcpy(pDataNew + (h * iWidth + w) * iChannelIn, ucRawData + (h * iWidth + w) * iChannelCurrent, iChannelCurrent);
            }
        }

        STBI_FREE(ucRawData);
        ucRawData = pDataNew;
        iChannelCurrent = iChannelIn;
    }
}

wstring FileLoader::GetLastDirectoryName(const filesystem::path& filePath)
{
    const wstring& wstrPathName = filePath.native();
    wstring::size_type pathNameStart = wstrPathName.find_last_of(L"\\");
    return wstrPathName.substr(pathNameStart + 1);
}

std::string FileLoader::ConvertWCharToChar(const std::wstring& pwStr)
{
    const wchar_t* pwChar = pwStr.c_str();
    char* pStr;
    int strSize = WideCharToMultiByte(CP_ACP, 0, pwChar, -1, NULL, 0, NULL, NULL);
    pStr = new char[strSize];
    WideCharToMultiByte(CP_ACP, 0, pwChar, -1, pStr, strSize, 0, 0);
    string resultStr = string(pStr);
    delete pStr;
    return resultStr;
}