#include "DirectXDevice.h"
#include "ID3D11Helper.h"

ID3D11Device* DirectXDevice::pDevice = nullptr;
ID3D11DeviceContext* DirectXDevice::pDeviceContext = nullptr;
IDXGISwapChain* DirectXDevice::pSwapChain = nullptr;

Microsoft::WRL::ComPtr<ID3D11Device> DirectXDevice::cpDevice;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> DirectXDevice::cpDeviceContext;
Microsoft::WRL::ComPtr<IDXGISwapChain> DirectXDevice::cpSwapChain;

void DirectXDevice::InitDevice(
	IN const UINT& uiWidthIn, 
	IN const UINT& uiHeightIn, 
	IN const DXGI_FORMAT& eFormatIn, 
	IN const bool& bWindowed, 
	IN HWND hOutputWindow
)
{
	ID3D11Helper::CreateDeviceAndContext(
		uiWidthIn,
		uiHeightIn,
		eFormatIn,
		bWindowed,
		hOutputWindow,
		cpDevice,
		cpDeviceContext,
		cpSwapChain
	);

	pDevice = cpDevice.Get();
	pDeviceContext = cpDeviceContext.Get();
	pSwapChain = cpSwapChain.Get();
}
