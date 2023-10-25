#include "ModelTextureLoader.h"
#include "ID3D11Helper.h"
#include "FileLoader.h"

using namespace std;
using namespace Microsoft::WRL;

extern void Console(const char* text);

string ModelTexture::strTextureType[5] = { string("Ambient Occulusion"), string("Diffuse"), string("Height"), string("Normal"), string("Reflect")};

ModelTexture::ModelTexture(
    const std::wstring& wStrNameIn,
    const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& cpModelTextureSRVIn
)
    : wStrModelTextureName(wStrNameIn)
{
    cpModelTextureSRVIn.CopyTo(cpModelTextureSRV.GetAddressOf());
}

ModelTexture::~ModelTexture()
{
    cpModelTextureSRV.Reset();
}

ModelTextureLoader::ModelTextureLoader(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
    : cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{

}

ModelTextureLoader::~ModelTextureLoader()
{

}


void ModelTextureLoader::LoadImageFromFile(IN const wstring& sPathName)
{
    filesystem::path texturePath(sPathName);
    if (filesystem::exists(texturePath) && filesystem::is_directory(texturePath))
    {
        sModelTextureSet.wStrDirectoryName = FileLoader::GetLastDirectoryName(texturePath);
        filesystem::directory_iterator dirIter(texturePath);
        int iWidth, iHeight, iChannel;
        uint8_t* ucImageRawData;

        uint8_t uIdx = 0;
        for (auto& entry : dirIter)
        {
            const wstring& wstrFileName = FileLoader::GetLastDirectoryName(entry.path());
            ucImageRawData = FileLoader::stbiw_load(entry.path().c_str(), &iWidth, &iHeight, &iChannel, 0);
            FileLoader::ExtendChannel(ucImageRawData, iWidth, iHeight, iChannel, 4);
            ComPtr<ID3D11Texture2D> cpImageTempTexture2D;
            ComPtr<ID3D11ShaderResourceView> cpImageShaderResource;

            ID3D11Helper::CreateTexture2D(cpDevice.Get(), cpDeviceContext.Get(), iWidth, iHeight, ucImageRawData, cpImageTempTexture2D.GetAddressOf());
            ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpImageTempTexture2D.Get(), cpImageShaderResource.GetAddressOf());
            cpDeviceContext->GenerateMips(cpImageShaderResource.Get());
            sModelTextureSet.sTextures[uIdx] = ModelTexture(wstrFileName, cpImageShaderResource);
            FileLoader::stbiw_free(ucImageRawData);
            uIdx++;
        }   
    }
}

