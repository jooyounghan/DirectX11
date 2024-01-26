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
		Console::AssertPrint("Swap Chain���κ��� Back Buffer�� ������ �� �����ϴ�.");
	}
}

void SwapChainAccessable::ResetBackBufferAddress()
{
	p_back_buffer = nullptr;
}
