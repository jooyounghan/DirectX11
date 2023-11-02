#include "FileLoader.h"
#define __STDC_LIB_EXT1__
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <atlconv.h>

using namespace std;

uint8_t* FileLoader::stbi_load(char const* sFilename, int* x, int* y, int* comp, int req_comp)
{
    FILE* f;
    fopen_s(&f, sFilename, "rb");
    uint8_t* result;
    if (!f) return stbi__errpuc("can't fopen", "Unable to open file");
    result = stbi_load_from_file(f, x, y, comp, req_comp);
    fclose(f);
    return result;
}

uint8_t* FileLoader::stbiw_load(const wchar_t* wFilename, int* x, int* y, int* comp, int req_comp)
{
    FILE* f;
    _wfopen_s(&f, wFilename, L"rb");
    uint8_t* result;
    if (!f) return stbi__errpuc("can't fopen", "Unable to open file");
    result = stbi_load_from_file(f, x, y, comp, req_comp);
    fclose(f);
    return result;
}

void FileLoader::stbi_free(uint8_t* pRawData)
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

//std::string FileLoader::ConvertUniCodeToAnsi(const std::wstring& wpStr)
//{
//    USES_CONVERSION;
//    return std::string(W2A(wpStr.c_str()));
//}
//
//std::wstring FileLoader::ConvertAnsiToUniCode(const std::string& pStr)
//{
//    USES_CONVERSION;
//    return std::wstring(A2W(pStr.c_str()));
//}

std::wstring FileLoader::ConvertUTF8ToUniCode(const std::string& str)
{
    int iUnicodeSize = ::MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        str.c_str(), static_cast<int>(str.size()),
        nullptr, 0
    );

    wstring result;
    result.resize(iUnicodeSize);

    ::MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        str.c_str(), static_cast<int>(str.size()),
        const_cast<wchar_t*>(result.c_str()), static_cast<int>(result.size())
    );

    return result;
}

std::string FileLoader::ConvertUniCodeToUTF8(const std::wstring& wStr)
{
    int iUTF8Size = ::WideCharToMultiByte(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        wStr.c_str(), lstrlenW(wStr.c_str()),
        NULL, 0, NULL, NULL
    );

    string result;
    result.resize(iUTF8Size);

    ::WideCharToMultiByte(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        wStr.c_str(), lstrlenW(wStr.c_str()),
        const_cast<char*>(result.c_str()), static_cast<int>(result.size()),
        NULL, NULL
    );

    return result;
}
