#include "SwapChainAccessable.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

SwapChainAccessable::SwapChainAccessable() 
	: p_back_buffer(nullptr)
{

}

SwapChainAccessable::~SwapChainAccessable()
{
}

void SwapChainAccessable::SetAsBackBufferAddress()
{
	if (!ID3D11Helper::GetBackBuffer(DirectXDevice::pSwapChain, &p_back_buffer))
	{
		Console::AssertPrint("Swap Chain으로부터 Back Buffer를 가져올 수 없습니다.");
	}
}

void SwapChainAccessable::ResetBackBufferAddress()
{
	if (p_back_buffer != nullptr)
	{
		p_back_buffer->Release();
		p_back_buffer = nullptr;
	}
}
