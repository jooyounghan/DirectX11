#include "FileInterface.h"
#include "ID3D11Helper.h"

FileInterface::FileInterface(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const std::string& strFileNameIn,
	uint8_t* pThumbNailDataIn,
	DXGI_FORMAT eThumbNailFormatIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), strFileName(strFileNameIn), eThumbNailFormat(eThumbNailFormatIn)
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), cpDeviceContext.Get(), 64, 64, eThumbNailFormatIn, pThumbNailDataIn, cpFileThumbNailTexture2D.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpFileThumbNailTexture2D.Get(), cpFileThumbNailSRV.GetAddressOf());
}
