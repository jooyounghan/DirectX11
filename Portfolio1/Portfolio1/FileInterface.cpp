#include "FileInterface.h"
#include "ID3D11Helper.h"

using namespace std;

FileInterface::FileInterface(
	const FileType& strFileTypeIn,
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const string& strFileNameIn,
	uint8_t* pThumbNailDataIn,
	DXGI_FORMAT eThumbNailFormatIn
)
	: eFileType(strFileTypeIn), cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), strFileName(strFileNameIn), eThumbNailFormat(eThumbNailFormatIn)
{
	if (pThumbNailDataIn)
	{
		ID3D11Helper::CreateTexture2D(cpDevice.Get(), cpDeviceContext.Get(), 64, 64, eThumbNailFormatIn, pThumbNailDataIn, cpFileThumbNailTexture2D.GetAddressOf());
		ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpFileThumbNailTexture2D.Get(), cpFileThumbNailSRV.GetAddressOf());
	}
}

FileInterface::FileInterface(
	const FileType& strFileTypeIn,
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const std::string& strFileNameIn
)
	: eFileType(strFileTypeIn), cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), strFileName(strFileNameIn), eThumbNailFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
{
}
