#include "ID3D11Helper.h"

void ID3D11Helper::CreateDeviceAndContext(HWND hOutputWindow, const int& iWidth, const int& iHeight, bool bWindowed, IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext)
{
	DXGI_SWAP_CHAIN_DESC sdSwapChainDesc;
	ZeroMemory(&sdSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	sdSwapChainDesc.BufferDesc.Width = 0;
	sdSwapChainDesc.BufferDesc.Height = 0;
	sdSwapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	sdSwapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	sdSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sdSwapChainDesc.SampleDesc.Count = 1;
	sdSwapChainDesc.SampleDesc.Quality = 0;
	sdSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sdSwapChainDesc.BufferCount = 2;
	sdSwapChainDesc.OutputWindow = hOutputWindow;
	sdSwapChainDesc.Windowed = bWindowed;
	sdSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sdSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	HRESULT hResult = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL /* D3D_FEATURE_LEVEL_11_0, 10_1, 10_0, 9_3, 9_2, 9_1 */,
		1,
		D3D11_SDK_VERSION,
		&sdSwapChainDesc,
		ppSwapChain,
		ppDevice,
		NULL,
		ppDeviceContext
	);
	
}
