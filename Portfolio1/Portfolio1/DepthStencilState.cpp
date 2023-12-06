#include "DepthStencilState.h"
#include "DefineVar.h"

extern void Console(const char*);

using namespace Microsoft::WRL;

DepthStencilState::DepthStencilState(ID3D11Device* pDevice)
{
	DepthStencilState::CreateDepthStencilState(DepthStencilOption::MaskOption, pDevice, DepthStencilState::cpMaskDSS.GetAddressOf());
	DepthStencilState::CreateDepthStencilState(DepthStencilOption::DrawEqualOption, pDevice, DepthStencilState::cpDrawEqualDSS.GetAddressOf());
	DepthStencilState::CreateDepthStencilState(DepthStencilOption::DrawNotEqualOption, pDevice, DepthStencilState::cpDrawNotEqualDSS.GetAddressOf());
	DepthStencilState::CreateDepthStencilState(DepthStencilOption::DefaultOption, pDevice, DepthStencilState::cpDefaultDSS.GetAddressOf());
}

ID3D11DepthStencilState* DepthStencilState::pGetDSS(IN DepthStencilOption eDSType)
{
	switch (eDSType)
	{
	case DepthStencilOption::MaskOption:
		return cpMaskDSS.Get();
	case DepthStencilOption::DrawEqualOption:
		return cpDrawEqualDSS.Get();
	case DepthStencilOption::DrawNotEqualOption:
		return cpDrawNotEqualDSS.Get();
	case DepthStencilOption::DefaultOption:
	default:
		return cpDefaultDSS.Get();
	}
}

void DepthStencilState::CreateDepthStencilState(IN DepthStencilOption eDSType, IN ID3D11Device* pDevice, OUT ID3D11DepthStencilState** ppDepthStencilState)
{
	D3D11_DEPTH_STENCIL_DESC sDepthStencilDesc;
	AutoZeroMemory(sDepthStencilDesc);

	sDepthStencilDesc.DepthEnable = TRUE;
	sDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	sDepthStencilDesc.StencilEnable = TRUE;
	sDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	sDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	sDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	sDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	sDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	sDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	sDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	sDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	sDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	sDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	switch (eDSType)
	{
	case MaskOption:
		sDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		sDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		sDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		sDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DrawEqualOption:
		sDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		sDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		sDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		sDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		sDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DrawNotEqualOption:
		sDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		sDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		sDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		sDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		sDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DefaultOption:
	default:
		break;
	}
	HRESULT hResult = pDevice->CreateDepthStencilState(&sDepthStencilDesc, ppDepthStencilState);

	if (FAILED(hResult))
	{
		Console("Depth Stencil State를 생성하는데 실패하였습니다.");
	}
}
