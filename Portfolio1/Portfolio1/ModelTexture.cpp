#include "ModelTexture.h"
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