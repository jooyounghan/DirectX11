#include "ASwapChainAccessable.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ASwapChainAccessable::ASwapChainAccessable() 
	: p_back_buffer(nullptr)
{

}

ASwapChainAccessable::~ASwapChainAccessable()
{
}

void ASwapChainAccessable::SetAsBackBufferAddress()
{
	if (!ID3D11Helper::GetBackBuffer(DirectXDevice::pSwapChain, &p_back_buffer))
	{
		Console::AssertPrint("Swap Chain으로부터 Back Buffer를 가져올 수 없습니다.");
	}
}

void ASwapChainAccessable::ResetBackBufferAddress()
{
	p_back_buffer = nullptr;
}
