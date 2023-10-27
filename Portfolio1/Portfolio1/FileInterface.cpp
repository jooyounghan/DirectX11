#include "FileInterface.h"
#include "ID3D11Helper.h"

FileInterface::FileInterface(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const std::string& strNameIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	uint8_t* pDataIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), strFileName(strNameIn)
{
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), cpDeviceContext.Get(), uiWidthIn, uiHeightIn, pDataIn, cpFileThumbNailTexture2D.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpFileThumbNailTexture2D.Get(), cpFileThumbNailSRV.GetAddressOf());
}
