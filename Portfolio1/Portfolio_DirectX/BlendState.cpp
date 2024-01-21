#include "BlendState.h"
#include "DefineVar.h"
#include "Console.h"


using namespace Microsoft::WRL;

float BlendState::DefaultBlendFactor[4] = { 1.f, 1.f, 1.f, 1.f };

BlendState::BlendState(ID3D11Device* pDevice) 
{
    BlendState::CreateBlendState(BlendStateOption::DefaultBlendState, pDevice, cpDefaultBS.GetAddressOf());
	BlendState::CreateBlendState(BlendStateOption::AddBlendState, pDevice, cpAdditionBS.GetAddressOf());
}

ID3D11BlendState* BlendState::pGetBS(IN BlendStateOption eBSType)
{
	switch (eBSType)
	{
	case BlendStateOption::AddBlendState:
		return cpAdditionBS.Get();
	case BlendStateOption::DefaultBlendState:
	default:
		return cpDefaultBS.Get();
	}
}

void BlendState::CreateBlendState(IN BlendStateOption eBSType, IN ID3D11Device* pDevice, OUT ID3D11BlendState** ppBlendState)
{
	D3D11_BLEND_DESC sBlendDesc;
	AutoZeroMemory(sBlendDesc);

	sBlendDesc.AlphaToCoverageEnable = true;
	sBlendDesc.IndependentBlendEnable = false;

	switch (eBSType)
	{

	case AddBlendState:
		sBlendDesc.IndependentBlendEnable = true;
		sBlendDesc.RenderTarget[0].BlendEnable = true;
		sBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		sBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		sBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		sBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		sBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		sBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		sBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		sBlendDesc.RenderTarget[1].BlendEnable = true;
		sBlendDesc.RenderTarget[1].SrcBlend = D3D11_BLEND_ONE;
		sBlendDesc.RenderTarget[1].DestBlend = D3D11_BLEND_ZERO;
		sBlendDesc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;

		sBlendDesc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
		sBlendDesc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
		sBlendDesc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		sBlendDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case DefaultBlendState:
	default:
		sBlendDesc.RenderTarget[0].BlendEnable = false;
		sBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		sBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		sBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		sBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		sBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		sBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		sBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}

	HRESULT hResult = pDevice->CreateBlendState(&sBlendDesc, ppBlendState);
	if (FAILED(hResult))
	{
		Console::AssertPrint("Blend State를 생성하는데 실패하였습니다.");
	}
}