#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include <algorithm>

ID3D11Device* DirectXDevice::pDevice = nullptr;
ID3D11DeviceContext* DirectXDevice::pDeviceContext = nullptr;
IDXGISwapChain* DirectXDevice::pSwapChain = nullptr;

Microsoft::WRL::ComPtr<ID3D11Device> DirectXDevice::cpDevice;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> DirectXDevice::cpDeviceContext;
Microsoft::WRL::ComPtr<IDXGISwapChain> DirectXDevice::cpSwapChain;

ID3D11Debug* DirectXDevice::pDebug = nullptr;
ID3D11InfoQueue* DirectXDevice::pDebugInfoQueue = nullptr;
std::vector<D3D11_MESSAGE_ID> DirectXDevice::vDebugMessages;

ID3D11SamplerState** DirectXDevice::ppWrapSampler = nullptr;
ID3D11SamplerState** DirectXDevice::ppClampSampler = nullptr;
ID3D11SamplerState** DirectXDevice::ppBorderSampler = nullptr;

Microsoft::WRL::ComPtr<ID3D11SamplerState> DirectXDevice::cpWrapSampler;
Microsoft::WRL::ComPtr<ID3D11SamplerState> DirectXDevice::cpClampSampler;
Microsoft::WRL::ComPtr<ID3D11SamplerState> DirectXDevice::cpBorderSampler;

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


	ID3D11Helper::CreateSampler(
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		NULL, pDevice,
		cpWrapSampler.GetAddressOf()
	);
	ppWrapSampler = cpWrapSampler.GetAddressOf();

	ID3D11Helper::CreateSampler(
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		NULL, pDevice,
		cpClampSampler.GetAddressOf()
	);
	ppClampSampler = cpClampSampler.GetAddressOf();

	float fBorderColor[4] = { 1.f, 1.f, 1.f, 1.f };
	ID3D11Helper::CreateSampler(
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_BORDER,
		fBorderColor, pDevice,
		cpBorderSampler.GetAddressOf()
	);
	ppBorderSampler = cpBorderSampler.GetAddressOf();
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
		pDebugInfoQueue->ClearStorageFilter();
		pDebugInfoQueue->AddStorageFilterEntries(&filter);
	}
}
