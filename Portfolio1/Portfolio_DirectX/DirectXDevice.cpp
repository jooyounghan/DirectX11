#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include <algorithm>

ID3D11Device* DirectXDevice::pDevice = nullptr;
ID3D11DeviceContext* DirectXDevice::pDeviceContext = nullptr;
IDXGISwapChain* DirectXDevice::pSwapChain = nullptr;

ID3D11Debug* DirectXDevice::pDebug = nullptr;
ID3D11InfoQueue* DirectXDevice::pDebugInfoQueue = nullptr;
std::vector<D3D11_MESSAGE_ID> DirectXDevice::vDebugMessages;

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

	HRESULT hr = pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDebug));
	if (SUCCEEDED(hr))
	{
		hr = pDebug->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&pDebugInfoQueue));
	}
}

void DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID eMessage)
{
	bool isAlreadAdded = false;
	for (D3D11_MESSAGE_ID& message : vDebugMessages)
	{
		if (message == eMessage)
		{
			isAlreadAdded = true;
			break;
		}
		else
		{
			continue;
		}
	}
	if (!isAlreadAdded)
	{
		vDebugMessages.push_back(eMessage);
	}
}

void DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID eMessage)
{
	std::erase_if(vDebugMessages, [=](D3D11_MESSAGE_ID& message) {return message == eMessage; });
}

void DirectXDevice::ApplyDebugMessageFilter()
{
	D3D11_INFO_QUEUE_FILTER filter = {};
	filter.DenyList.NumIDs = (UINT)vDebugMessages.size();
	filter.DenyList.pIDList = vDebugMessages.data();

	if (pDebugInfoQueue != nullptr)
	{
		pDebugInfoQueue->AddStorageFilterEntries(&filter);
	}
}
