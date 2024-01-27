#pragma once
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>

class DebugHelper
{
public:
	static std::vector<D3D11_MESSAGE_ID> debug_message;

public:
	static void AddIgoreMessage(ID3D11Device* pDeivceIn, D3D11_MESSAGE_ID id)
	{
		debug_message.clear();
		debug_message.push_back(id);

		ID3D11Debug* debugDevice = nullptr;
		HRESULT hr = pDeivceIn->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));

		ID3D11InfoQueue* d3dInfoQueue = nullptr;
		if (SUCCEEDED(hr))
		{
			hr = debugDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&d3dInfoQueue));

			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = debug_message.size();
			filter.DenyList.pIDList = debug_message.data();

			// ���͸� �����Ͽ� Ư�� ��� �޽����� �ٽ� ǥ���մϴ�.
			d3dInfoQueue->AddStorageFilterEntries(&filter);

		}
		if (d3dInfoQueue)
			d3dInfoQueue->Release();
		if (debugDevice)
			debugDevice->Release();

	}
	static void ResetFilter(ID3D11Device* pDeivceIn)
	{
		ID3D11Debug* debugDevice = nullptr;
		HRESULT hr = pDeivceIn->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));

		ID3D11InfoQueue* d3dInfoQueue = nullptr;
		if (SUCCEEDED(hr))
		{
			hr = debugDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&d3dInfoQueue));
			d3dInfoQueue->ClearStorageFilter();
			d3dInfoQueue->ClearRetrievalFilter();
		}
		if (d3dInfoQueue)
			d3dInfoQueue->Release();
		if (debugDevice)
			debugDevice->Release();
	}
};

