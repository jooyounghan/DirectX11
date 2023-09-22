#include "FileManager.h"

#define __STDC_LIB_EXT1__
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "ID3D11Helper.h"

using namespace std;
using namespace Microsoft::WRL;

extern void Console(const char* text);


FileManager::FileManager(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
    : cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
}

FileManager::~FileManager()
{
}

void FileManager::LoadImageFromFile(IN const wstring& sPathName)
{
    filesystem::path texturePath(sPathName);
    if (filesystem::exists(texturePath) && filesystem::is_directory(texturePath))
    {
        TexturesInDirectory sTextureSet;
        sTextureSet.wstrTextureName = GetLastDirectoryName(texturePath);
        filesystem::directory_iterator dirIter(texturePath);
        int iWidth, iHeight, iChannel;
        uint8_t* ucImageRawData;

        for (auto& entry : dirIter)
        {
            const wstring& wstrFileName = GetLastDirectoryName(entry.path());
            ucImageRawData = stbiw_load(entry.path().c_str(), &iWidth, &iHeight, &iChannel, 0);
            ExtendChannel(ucImageRawData, iWidth, iHeight, iChannel, 4);
            ComPtr<ID3D11Texture2D> cpImageTempTexture2D;
            ComPtr<ID3D11ShaderResourceView> cpImageShaderResource;

            ID3D11Helper::CreateTexture2D(cpDevice.Get(), cpDeviceContext.Get(), iWidth, iHeight, ucImageRawData, cpImageTempTexture2D.GetAddressOf());
            ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpImageTempTexture2D.Get(), cpImageShaderResource.GetAddressOf());
            cpDeviceContext->GenerateMips(cpImageShaderResource.Get());
            sTextureSet.ImageShaderResources.push_back(cpImageShaderResource);
        }   
        vTextures.push_back(sTextureSet);
    }
}

uint8_t* FileManager::stbiw_load(wchar_t const* wFilename, int* x, int* y, int* comp, int req_comp)
{
    FILE* f = _wfopen(wFilename, L"rb");
    uint8_t* result;
    if (!f) return stbi__errpuc("can't fopen", "Unable to open file");
    result = stbi_load_from_file(f, x, y, comp, req_comp);
    fclose(f);
    return result;
}

void FileManager::ExtendChannel(uint8_t*& ucRawData, const int& iWidth, const int& iHeight, int& iChannelCurrent, const int& iChannelIn)
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

wstring FileManager::GetLastDirectoryName(const filesystem::path& filePath)
{
    const wstring& wstrPathName = filePath.native();
    wstring::size_type pathNameStart = wstrPathName.find_last_of(L"\\");
    return wstrPathName.substr(pathNameStart + 1);
}
