#include "ModelTextureFile.h"
#include "ID3D11Helper.h"
#include "FileLoader.h"

using namespace std;
using namespace Microsoft::WRL;

extern void Console(const char* text);

string ModelTextureFile::strTextureType[MODEL_TEXTURE_NUM] = { string("Ambient Occulusion"), string("Color"), string("Metalness"), string("Roughness"), string("Normal"), string("Height") };

ModelTextureFile::ModelTextureFile(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const std::string& strFileNameIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	uint8_t* pTextureDataIn,
	DXGI_FORMAT eThumbNailFormatIn
)
	: FileInterface(cpDeviceIn, cpDeviceContextIn, strFileNameIn, nullptr, eThumbNailFormatIn)
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), cpDeviceContext.Get(), uiWidthIn, uiHeightIn, eThumbNailFormat, pTextureDataIn, cpModelTextureTexture2D.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpModelTextureTexture2D.Get(), cpModelTextureSRV.GetAddressOf());
	cpDeviceContext->GenerateMips(cpModelTextureSRV.Get());

	cpModelTextureTexture2D.CopyTo(cpFileThumbNailTexture2D.GetAddressOf());
	cpModelTextureSRV.CopyTo(cpFileThumbNailSRV.GetAddressOf());
}

ModelTextureFile::~ModelTextureFile()
{

}